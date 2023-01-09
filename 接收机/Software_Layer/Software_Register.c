#include "Software_Register.h"

/**
*@Description :软件寄存器初始化
*@param       :软件寄存器指针
**/
void Software_Register_Init(Software_Register *Register)
{
	uint8_t i;
	for(i=0;i<Register->Register_Len;i++)
	{
		Register->Permission[i] = Read_Write;
		Register->Register[i] = 0;
	}
}
/**
*@Description :软件寄存器读操作
*@param       :软件寄存器指针
*@param       :待操作寄存器地址
*@param       :待读出数据指针
*@return      :操作状态
**/
bool Software_Register_Read(Software_Register *Register,uint8_t addr,uint8_t *data)
{
	if(Register->Register_Len<=addr)
		return false;
	if(Register->Permission[addr] <= Read_Write)
	{
		*data =  Register->Register[addr];
		return true;
	}
	return false;
}
/**
*@Description :软件寄存器写操作
*@param       :软件寄存器指针
*@param       :待操作寄存器地址
*@param       :待写入数据指针
*@return      :操作状态
**/
bool Software_Register_Write(Software_Register *Register,uint8_t addr,uint8_t *data)
{
	if(Register->Register_Len<=addr)
		return false;
	if(Register->Permission[addr] >= Read_Write)
	{
		Register->Register[addr] = *data;
		return true;
	}
	return false;
}
/**
*@Description :软件寄存器连续读操作
*@param       :软件寄存器指针
*@param       :待操作寄存器地址
*@param       :待读出数据指针
*@param       :待读出数据量
*@return      :成功读出数量
**/
uint8_t Software_Register_ReadLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len)
{
	uint8_t temp = 0;
	if(Register->Register_Len<=(addr+Len))
		return 0;
	for(;temp<Len;)
	{
		if(Register->Permission[temp+addr] <= Read_Write)
		{
			data[temp] = Register->Register[temp+addr];
			temp++;
		}else
		{
			break;
		}
	}
	return temp;
}
/**
*@Description :软件寄存器连续写操作
*@param       :软件寄存器指针
*@param       :待操作寄存器地址
*@param       :待写入数据指针
*@param       :待写入数据量
*@return      :成功写入数量
**/
uint8_t Software_Register_WriteLen(Software_Register *Register,uint8_t addr,uint8_t *data,uint8_t Len)
{
	uint8_t temp = 0;
	if(Register->Register_Len<=(addr+Len))
		return 0;
	for(;temp<Len;)
	{
		if(Register->Permission[temp+addr] >= Read_Write)
		{
			Register->Register[temp+addr] = data[temp];
			temp++;
		}else
		{
			break;
		}
	}
	return temp;
}






