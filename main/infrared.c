#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp32/rom/uart.h"

#include "infrared.h"
#include "ir_encoder.h"
#include "ir_decoder.h"
#include "states.h"
#include "telemetry.h"
#include "wifi.h"
#include "mqtt.h"

#define IR_RESOLUTION_HZ 1000000 // 1MHz resolution, 1 tick = 1us

#define IR_TX_GPIO_NUM 17
#define IR_RX_GPIO_NUM 16
#define BUTTON_GPIO_NUM 0

#define TAG "INFRARED"

static uint16_t code_address;
static uint16_t code_command;

static void parse_signal(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num, State *state)
{
    switch (symbol_num)
    {
    case 34: // NEC normal frame
        if (decode_signal(rmt_nec_symbols, &code_address, &code_command) && state != NULL)
        {
            int BUTTON_STATE = gpio_get_level(BUTTON_GPIO_NUM);
            if (code_address == 0X01 && !BUTTON_STATE)
            {
                ESP_LOGW(TAG, "Exiting low power mode");
                state->low_power = 0;
                // wifi_start();
                mqtt_start();
            }
            else if (code_address == 0X01 && !state->low_power)
            {
                ESP_LOGW(TAG, "Starting low power mode 1");
                state->low_power = 1;
                uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
                esp_light_sleep_start();
            }
            else
            {
                if (state->mode == IR_MODE)
                {
                    joystick_set_percent(&state->joystick, code_command & 0xFF, (code_command & 0xFEFE) >> 8);
                }

                ESP_LOGI(TAG, "Address=%04X, Command=%d, X=%d, Y=%d\r\n\r\n", code_address, code_command, state->joystick.x_percent, state->joystick.y_percent);
            }
        }
        break;
    case 2: // NEC repeat frame
        if (decode_signal_repeat(rmt_nec_symbols))
        {
            ESP_LOGI(TAG, "Address=%04X, Command=%04X, repeat\r\n\r\n", code_address, code_command);
        }
        break;
    default:
        ESP_LOGW(TAG, "Unknown NEC frame\r\n\r\n");
    }
}

static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

void infrared_tx_task(void *params)
{
    State *state = (State *)params;

    ESP_LOGI(TAG, "create RMT TX channel");
    rmt_tx_channel_config_t tx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .trans_queue_depth = 4,  // number of transactions that allowed to pending in the background, this example won't queue multiple transactions, so queue depth > 1 is sufficient
        .gpio_num = IR_TX_GPIO_NUM,
    };
    rmt_channel_handle_t tx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

    ESP_LOGI(TAG, "modulate carrier to TX channel");
    rmt_carrier_config_t carrier_cfg = {
        .duty_cycle = 0.33,
        .frequency_hz = 38000, // 38KHz
    };
    ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

    rmt_transmit_config_t transmit_config = {
        .loop_count = 0, // no loop
    };

    ESP_LOGI(TAG, "install IR NEC encoder");
    ir_nec_encoder_config_t nec_encoder_cfg = {
        .resolution = IR_RESOLUTION_HZ,
    };
    rmt_encoder_handle_t nec_encoder = NULL;
    ESP_ERROR_CHECK(rmt_new_ir_nec_encoder(&nec_encoder_cfg, &nec_encoder));

    ESP_ERROR_CHECK(rmt_enable(tx_channel));

    ir_nec_scan_code_t scan_code = {
        .address = 0x0440,
        .command = 0,
    };
    while (1)
    {
        if (state->mode == IR_MODE)
        {
            scan_code.address = state->joystick.power_switch ? 0x0440 : 0x01;
            scan_code.command = state->joystick.x_percent + (state->joystick.y_percent << 8);
            ESP_LOGI(TAG, "Sending joystick %d", scan_code.command);
            ESP_ERROR_CHECK(rmt_transmit(tx_channel, nec_encoder, &scan_code, sizeof(scan_code), &transmit_config));
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void infrared_rx_task(void *params)
{
    State *state = (State *)params;

    ESP_LOGI(TAG, "create RMT RX channel");

    rmt_rx_channel_config_t rx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .gpio_num = IR_RX_GPIO_NUM,
    };

    gpio_set_direction(BUTTON_GPIO_NUM, GPIO_MODE_INPUT);
    gpio_wakeup_enable(BUTTON_GPIO_NUM, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    rmt_channel_handle_t rx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

    ESP_LOGI(TAG, "register RX done callback");
    QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
    assert(receive_queue);
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = rmt_rx_done_callback,
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

    // the following timing requirement is based on NEC protocol
    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250,     // the shortest duration for NEC signal is 560us, 1250ns < 560us, valid signal won't be treated as noise
        .signal_range_max_ns = 12000000, // the longest duration for NEC signal is 9000us, 12000000ns > 9000us, the receive won't stop early
    };

    ESP_ERROR_CHECK(rmt_enable(rx_channel));

    // save the received RMT symbols
    rmt_symbol_word_t raw_symbols[64]; // 64 symbols should be sufficient for a standard NEC frame
    rmt_rx_done_event_data_t rx_data;

    ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
    while (1)
    {
        if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));

            parse_signal(rx_data.received_symbols, rx_data.num_symbols, state);
        }

        if (state->low_power)
        {
            ESP_LOGW(TAG, "Woke up");
            wifi_start();
            mqtt_start();
            telemetry_send_car_attributes(state);
            ESP_LOGW(TAG, "Sleeping");
            uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
            esp_light_sleep_start();
        }
    }
}
