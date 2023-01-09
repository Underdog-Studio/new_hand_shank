
#include "gd32f30x.h"
#include "core_cm4.h"




#ifndef _System_time_H
#define _System_time_H

typedef struct
{
	uint32_t s;
	uint16_t ms;
	uint16_t us;
}Time;


#ifdef __cplusplus
extern "C" {
#endif
void System_time_init(void);
//获取系统时间
Time Get_System_T(void);
//获取时间间隔
float Get_deta_T(Time T);
//ms延时
void delay_ms(uint16_t nms);
//us延时
void delay_us(uint32_t nus);

#ifdef __cplusplus
}
#endif













#endif



