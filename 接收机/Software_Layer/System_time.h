#ifndef _System_time_H
#define _System_time_H

#include "gd32e23x.h"
#include "core_cm23.h"
typedef struct
{
	uint32_t s;
	int16_t ms;
	int16_t us;
}Time;//时间数据

//滴答定时器初始化
void System_time_init(void);

#ifdef __cplusplus
extern "C" {
#endif
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



