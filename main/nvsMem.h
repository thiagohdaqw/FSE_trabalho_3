#ifndef __NVSMEM_H__
#define __NVSMEM_H__

int32_t le_valor_nvs();
void grava_valor_nvs(int32_t valor);
void nvs_main(void *params);

#endif