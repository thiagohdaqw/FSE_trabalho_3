idf_component_register(SRCS "ir_encoder.c" "main.c" "adc.c" "joystick.c" "temperature_analogic.c" "temperature_dht11.c" "infrared.c" "ir_encoder.c"
                    INCLUDE_DIRS "."
                    REQUIRES "esp32-DHT11" "esp_adc")
