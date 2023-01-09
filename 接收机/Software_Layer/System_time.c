/*****************************************************************
File name:滴答定时器系统实时时钟

Author:赵华磊

Version:V0.2

Description:提供系统时间 提供延时

Others:

Log:2021/4/28
*****************************************************************/

/*******头文件引用********/
#include "System_time.h"

/*************************/


/*****************************************************************************************
//
//使用前按以下内容进行修改
//
//滴答定时器配置函数修改 （中断优先级修改为最高）
//static __INLINE uint32_t SysTick_Config(uint32_t ticks)
//{ 
//  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            
//                                                               
//  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      
//   NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0,0));  
//  SysTick->VAL   = 0;                                        
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
//                   SysTick_CTRL_TICKINT_Msk   | 
//                   SysTick_CTRL_ENABLE_Msk;                   
//  return (0);                                                  
//}
//
//
//宏定义修改  (时钟修改为72m)
//#define SysTick_CTRL_TICKINT_Pos            1                                            
//#define SysTick_CTRL_TICKINT_Msk           (1ul << SysTick_CTRL_TICKINT_Pos)              
//
//
//
//
//
*****************************************************************************************/



/*
*@ Description 滴答定时器初始化初始化
*@ prarm 无参数
*@ return 无返回值
*/
static Time System_T;
void System_time_init(void)
{
	SysTick_Config(SystemCoreClock/1000);	
	System_T.s=0;
	System_T.ms=0;
	System_T.us=0;
}

/*
*@ Description 滴答定时器中断处理函数
*@ prarm 无参数
*@ return 无返回值
*/
void SysTick_Handler(void)
{
		System_T.ms++;
		if(System_T.ms>=1000)
		{
			System_T.ms=0;
			System_T.s++;
		}
}

/*
*@ Description 获取系统当前时间
*@ prarm 无参数
*@ return 系统当前时间
*/
Time Get_System_T(void)
{
	uint8_t i=0;
	System_T.us=1000-((SysTick->VAL)/108);
	i++;
	return System_T;
}

/*
*@ Description 获取时间间隔 单位S
*@ prarm 开始时间
*@ return 时间间隔
*/
float Get_deta_T(Time T)
{
	return (System_T.s-T.s)
					+(System_T.ms-T.ms)/1000.0f
					+(Get_System_T().us-T.us)/1000000.0f;
}

/*
*@ Description us延时函数
*@ prarm us时间
*@ return 无返回值
*/
void delay_us(uint32_t nus)
{
	Time Last_T=Get_System_T();
	while((Get_deta_T(Last_T))<=nus/1000000.0f);
}
	
/*
*@ Description ms延时函数
*@ prarm ms时间
*@ return 无返回值
*/
void delay_ms(uint16_t nms)
{
 	Time Last_T=Get_System_T();
	while((Get_deta_T(Last_T))<=nms/1000.0f);
}








