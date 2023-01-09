#include "timer_hal.h"


void timer_hal::timer__init(uint32_t timer_periph,
					uint16_t prescaler,//分频系数  
					uint32_t period,    //预装载值
					uint8_t nvic_irq_priority,	//中断优先级
					uint16_t alignedmode,//边沿对其                      
					uint16_t counterdirection,//向上计数模式                  
					uint16_t clockdivision,//时钟分频值                           
					uint8_t  repetitioncounter)
{
	this->timer_num = timer_periph;
	timer_parameter_struct timer_initparam;
	timer_deinit(this->timer_num);
	timer_initparam.prescaler=prescaler;                               
	timer_initparam.alignedmode=alignedmode;               
	timer_initparam.counterdirection=counterdirection;            
	timer_initparam.period=period;                               
	timer_initparam.clockdivision=clockdivision;              
	timer_initparam.repetitioncounter=repetitioncounter;            
	
	
	switch (this->timer_num)
	{
	case TIMER0:
		rcu_periph_clock_enable(RCU_TIMER0);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER0_UP_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	case TIMER1:
		rcu_periph_clock_enable(RCU_TIMER1);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER1_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	case TIMER2:
		rcu_periph_clock_enable(RCU_TIMER2);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
			nvic_irq_enable(TIMER2_IRQn,nvic_irq_priority,0);
		}
		break;
	case TIMER3:
		rcu_periph_clock_enable(RCU_TIMER3);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER3_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	case TIMER4:
		rcu_periph_clock_enable(RCU_TIMER4);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER4_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	case TIMER5:
		rcu_periph_clock_enable(RCU_TIMER5);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER5_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	case TIMER6:
		rcu_periph_clock_enable(RCU_TIMER6);
		timer_init(this->timer_num,&timer_initparam);
		if(nvic_irq_priority != 255)
		{
			timer_interrupt_flag_clear(this->timer_num,TIMER_INT_FLAG_UP);//清除中断标志位
			nvic_irq_enable(TIMER6_IRQn,nvic_irq_priority,0);
			timer_interrupt_enable(this->timer_num,TIMER_INT_UP);//使能数据更新中断
		}
		break;
	default:
		break;
	}
}
/*
*@ 设置自动重装载值
*@ 重装载值 0-65535
*/
void timer_hal::set_autoreload_value(uint16_t reload)
{
	TIMER_CAR(this->timer_num) = (uint32_t)reload;
}

/*
*@ 设置当前计数值
*@ 当前计数值 0-65535
*/
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
//定时器中断函数
static void (*TIMER0_Callback)(void*)=0;
void *timer0_str;
void TIMER0_UP_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER0,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER0_Callback(timer0_str);
		timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER1_Callback)(void*)=0;
void *timer1_str;
void TIMER1_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER1,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER1_Callback(timer1_str);
		timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_UP);
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

static void (*TIMER3_Callback)(void*)=0;
void *timer3_str;
void TIMER3_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER3,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER3_Callback(timer3_str);
		timer_interrupt_flag_clear(TIMER3,TIMER_INT_FLAG_UP);
	}
}

static void (*TIMER4_Callback)(void*)=0;
void *timer4_str;
void TIMER4_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER4,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER4_Callback(timer4_str);
		timer_interrupt_flag_clear(TIMER4,TIMER_INT_FLAG_UP);
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

static void (*TIMER6_Callback)(void*);
void *timer6_str;
void TIMER6_IRQHandler(void)
{
	if(timer_interrupt_flag_get(TIMER6,TIMER_INT_FLAG_UP)==SET)
	{		
		TIMER6_Callback(timer6_str);
		timer_interrupt_flag_clear(TIMER6,TIMER_INT_FLAG_UP);
	}
}

}





/*
*@ 定时器中断回调注册
*/
void timer_hal::Callback_Register(void *str,void(*app)(void*))
{
	switch (this->timer_num)
	{
	case TIMER0:
		TIMER0_Callback = app;
		timer0_str = str;
		break;
	case TIMER1:
		TIMER1_Callback = app;
		timer1_str = str;
		break;
	case TIMER2:
		TIMER2_Callback = app;
		timer2_str = str;
		break;
	case TIMER3:
		TIMER3_Callback = app;
		timer3_str = str;
		break;
	case TIMER4:
		TIMER4_Callback = app;
		timer4_str = str;
		break;
	case TIMER5:
		TIMER5_Callback = app;
		timer5_str = str;
		break;
	case TIMER6:
		TIMER6_Callback = app;
		timer6_str = str;
		break;
	default:
		break;
	}
}