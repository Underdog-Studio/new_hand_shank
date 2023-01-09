#ifndef _exti_hal_H
#define _exti_hal_H
#include "gd32e23x_it.h"
#include "gd32e23x_exti.h"
class exti_hal
{
private:
    exti_line_enum linex;
    uint8_t get_exti_source(uint32_t gpio_periph);
    uint8_t get_exti_gpiopin(exti_line_enum linex);
public:
    exti_hal()=default;
    void init(exti_line_enum linex,                              //中断线
              uint32_t gpio_periph,                              //GPIO端口
              uint8_t nvic_irq_priority=16,                      //中断优先级
              exti_mode_enum mode=EXTI_INTERRUPT,                //中断模式
              exti_trig_type_enum trig_type=EXTI_TRIG_FALLING,    //默认下降沿触发
              uint32_t pull_up_down=GPIO_PUPD_PULLUP               //默认无上下拉
              );
    void Callback_Register(void *str,void (*app)(void *str));
    void set_exti_trig(exti_trig_type_enum trig_type);
};


#define EXTI_Lin0_hal 1
#define EXTI_Lin1_hal 1
#define EXTI_Lin2_hal 1
#define EXTI_Lin3_hal 1
#define EXTI_Lin4_hal 1
#define EXTI_Lin5_hal 1
#define EXTI_Lin6_hal 1
#define EXTI_Lin7_hal 1
#define EXTI_Lin8_hal 1
#define EXTI_Lin9_hal 1
#define EXTI_Lin10_hal 1
#define EXTI_Lin11_hal 1
#define EXTI_Lin12_hal 1
#define EXTI_Lin13_hal 1
#define EXTI_Lin14_hal 1
#define EXTI_Lin15_hal 1


#endif
