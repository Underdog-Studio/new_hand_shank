#include "timer_hal.h"


void timer_hal::init(uint32_t timer_periph,
					uint32_t cycle,//频率
					uint32_t resolution,    //分辨率
					uint8_t nvic_irq_priority,	//中断优先级
					uint16_t alignedmode,//边沿对其                      
					uint16_t counterdirection,//向上计数模式                  
					uint16_t clockdivision,//时钟分频值                           
					uint8_t  repetitioncounter)
{
	this->timer_num = timer_periph;//(SystemCoreClock/(2*cycle*resolution) - 1),resolution-1)
	timer_parameter_struct timer_initparam;
	timer_deinit(this->timer_num);
	                               
	timer_initparam.alignedmode=alignedmode;               
	timer_initparam.counterdirection=counterdirection;            
	timer_initparam.period=resolution-1;                               
	timer_initparam.clockdivision=clockdivision;              
	timer_initparam.repetitioncounter=repetitioncounter;            
	
	switch (this->timer_num)
	{
	case TIMER0:
		timer_initparam.prescaler=(SystemCoreClock/(cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER0);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER0_BRK_UP_TRG_COM_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER0,TIMER_INT_UP);
		}
		break;
	case TIMER2:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER2);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER2_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER2,TIMER_INT_UP);
		}
		break;
	case TIMER5:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER5);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER5_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER5,TIMER_INT_UP);
		}
		break;
	case TIMER13:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER13);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER13_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER13,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER13,TIMER_INT_UP);
		}
		break;
	case TIMER14:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER14);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER14_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER14,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER14,TIMER_INT_UP);
		}
		break;
	case TIMER15:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER15);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER15_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER15,TIMER_INT_UP);
		}
		break;
	case TIMER16:
		timer_initparam.prescaler=(SystemCoreClock/(2*cycle*resolution) - 1);
		rcu_periph_clock_enable(RCU_TIMER16);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			nvic_irq_enable(TIMER16_IRQn,nvic_irq_priority);
			timer_interrupt_flag_clear(TIMER16,TIMER_INT_FLAG_UP);
			timer_interrupt_enable(TIMER16,TIMER_INT_UP);
		}
		break;
	default:
		break;
	}


}

void timer_hal::set_autoreload_value(uint16_t reload)
{
	TIMER_CAR(this->timer_num) = (uint32_t)reload;
}

void timer_hal::set_counter_value(uint16_t counter)
{
	TIMER_CNT(this->timer_num) = (uint32_t)counter;
}
/*
*@ 定时器开关
*@ ENABLE DISABLE
*/
void timer_hal::timer_switch(ControlStatus value)
{
	switch (value)
	{
	case ENABLE:
		TIMER_CTL0(this->timer_num) |= (uint32_t)TIMER_CTL0_CEN;
		break;
	case DISABLE:
		TIMER_CTL0(this->timer_num) &= ~(uint32_t)TIMER_CTL0_CEN;
		break;
	default:
		break;
	}
}

extern "C"{

static void (*TIMER0_Callback)(void*)=0;
void *timer0_str;
void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER0_Callback(timer0_str);
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER2_Callback)(void*)=0;
void *timer2_str;
void TIMER2_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER2,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER2_Callback(timer2_str);
		timer_interrupt_flag_clear(TIMER2,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER5_Callback)(void*);
void *timer5_str;
void TIMER5_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER5,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER5_Callback(timer5_str);
		timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER13_Callback)(void*);
void *timer13_str;
void TIMER13_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER13,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER13_Callback(timer13_str);
		timer_interrupt_flag_clear(TIMER13,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER14_Callback)(void*);
void *timer14_str;
void TIMER14_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER14,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER14_Callback(timer14_str);
		timer_interrupt_flag_clear(TIMER14,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER15_Callback)(void*);
void *timer15_str;
void TIMER15_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER15,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER15_Callback(timer15_str);
		timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER16_Callback)(void*);
void *timer16_str;
void TIMER16_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER16,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER16_Callback(timer16_str);
		timer_interrupt_flag_clear(TIMER16,TIMER_INT_FLAG_UP);
	}
}

}






void timer_hal::Callback_Register(void *str,void(*app)(void*))
{
	switch (this->timer_num)
	{
	case TIMER0:
		TIMER0_Callback = app;
		timer0_str = str;
		break;
	case TIMER2:
		TIMER2_Callback = app;
		timer2_str = str;
		break;
	case TIMER5:
		TIMER5_Callback = app;
		timer5_str = str;
		break;
	case TIMER13:
		TIMER13_Callback = app;
		timer13_str = str;
		break;
	case TIMER14:
		TIMER14_Callback = app;
		timer14_str = str;
		break;
	case TIMER15:
		TIMER15_Callback = app;
		timer15_str = str;
		break;
	case TIMER16:
		TIMER16_Callback = app;
		timer16_str = str;
		break;
	default:
		break;
	}
}