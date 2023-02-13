#ifndef __DECODER_H__
#define __DECODER_H__

bool decode_signal(rmt_symbol_word_t *rmt_nec_symbols, uint16_t *code_address, uint16_t *code_command);

bool decode_signal_repeat(rmt_symbol_word_t *rmt_nec_symbols);

#endif
