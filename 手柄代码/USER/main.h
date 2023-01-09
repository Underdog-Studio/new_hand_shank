#ifndef _MAIN_H
#define _MAIN_H

#include "gd32f30x_it.h"



//gd32标准库
#include "gd32f30x_libopt.h"


#ifdef __cplusplus
extern "C" {
#endif

void SPI_FLASH_Write(uint8_t *write_buffer,uint32_t write_addr,uint16_t write_len);
void SPI_FLASH_Read(uint8_t *read_buffer,uint32_t read_addr,uint16_t read_len);
void Main(void *pvParameters);
uint32_t keypad_get_key(void);
#ifdef __cplusplus
}
#endif

#endif
