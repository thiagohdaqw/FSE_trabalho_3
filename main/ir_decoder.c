


#include "freertos/FreeRTOS.h"
#include "driver/rmt_tx.h"

#define IR_NEC_DECODE_MARGIN 200 // Tolerance for parsing RMT symbols into bit stream

#define NEC_LEADING_CODE_DURATION_0 9000
#define NEC_LEADING_CODE_DURATION_1 4500
#define NEC_PAYLOAD_ZERO_DURATION_0 560
#define NEC_PAYLOAD_ZERO_DURATION_1 560
#define NEC_PAYLOAD_ONE_DURATION_0 560
#define NEC_PAYLOAD_ONE_DURATION_1 1690
#define NEC_REPEAT_CODE_DURATION_0 9000
#define NEC_REPEAT_CODE_DURATION_1 2250

static inline bool check_duration_in_range(uint32_t signal_duration, uint32_t spec_duration)
{
    return (signal_duration < (spec_duration + IR_NEC_DECODE_MARGIN)) &&
           (signal_duration > (spec_duration - IR_NEC_DECODE_MARGIN));
}

static bool is_zero(rmt_symbol_word_t *rmt_nec_symbols)
{
    return check_duration_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ZERO_DURATION_0) &&
           check_duration_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
}

static bool is_one(rmt_symbol_word_t *rmt_nec_symbols)
{
    return check_duration_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
           check_duration_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ONE_DURATION_1);
}

bool decode_signal_repeat(rmt_symbol_word_t *rmt_nec_symbols)
{
    return check_duration_in_range(rmt_nec_symbols->duration0, NEC_REPEAT_CODE_DURATION_0) &&
           check_duration_in_range(rmt_nec_symbols->duration1, NEC_REPEAT_CODE_DURATION_1);
}

bool decode_signal(rmt_symbol_word_t *rmt_nec_symbols, uint16_t *code_address, uint16_t *code_command)
{
    rmt_symbol_word_t *cur = rmt_nec_symbols;
    uint16_t address = 0;
    uint16_t command = 0;
    bool valid_leading_code = check_duration_in_range(cur->duration0, NEC_LEADING_CODE_DURATION_0) &&
                              check_duration_in_range(cur->duration1, NEC_LEADING_CODE_DURATION_1);
    if (!valid_leading_code)
    {
        return false;
    }
    cur++;
    for (int i = 0; i < 16; i++)
    {
        if (is_one(cur))
        {
            address |= 1 << i;
        }
        else if (is_zero(cur))
        {
            address &= ~(1 << i);
        }
        else
        {
            return false;
        }
        cur++;
    }
    for (int i = 0; i < 16; i++)
    {
        if (is_one(cur))
        {
            command |= 1 << i;
        }
        else if (is_zero(cur))
        {
            command &= ~(1 << i);
        }
        else
        {
            return false;
        }
        cur++;
    }
    // save address and command
    *code_address = address;
    *code_command = command;
    return true;
}
