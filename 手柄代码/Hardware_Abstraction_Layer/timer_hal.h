#ifndef _timer_hal_H
#define _timer_hal_H
#include "gd32f30x_libopt.h"

class timer_hal
{
private:
    uint32_t timer_num;
public:
    timer_hal()=default;
    void timer__init(uint32_t timer_periph,
              uint16_t prescaler,//分频系数  
              uint32_t period,    //预装载值
              uint8_t nvic_irq_priority=255,	//中断优先级
              uint16_t alignedmode=TIMER_COUNTER_EDGE,//边沿对其                      
              uint16_t counterdirection=TIMER_COUNTER_UP,//向上计数模式                  
              uint16_t clockdivision=TIMER_CKDIV_DIV1,//时钟分频值                           
              uint8_t  repetitioncounter = 0
              );
    void Callback_Register(void *str,void(*app)(void*));
    void timer_switch(ControlStatus value);
    void set_autoreload_value(uint16_t reload);
    void set_counter_value(uint16_t counter);
};







#endif

