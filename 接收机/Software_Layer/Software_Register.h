#ifndef __Software_Register_H
#define __Software_Register_H
#include "gd32e23x_it.h"
#include "stdbool.h"


typedef enum
{
	Read_Only=0,
	Read_Write=1,
	Write_Only=2
}Permissions;

typedef struct
{
  uint8_t *Register;//寄存器指针
	uint8_t Register_Len;//寄存器数量
	Permissions *Permission;//寄存器权限
}Software_Register;

void Software_Register_Init(Software_Register *Register);
bool Software_Register_Read(Software_Register *Register,uint8_t addr,uint8_t *data);
bool Software_Register_Write(Software_Register *Register,uint8_t addr,uint8_t *data);
uint8_t Software_Register_ReadLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len);
uint8_t Software_Register_WriteLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len);
#endif

