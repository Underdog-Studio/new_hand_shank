#ifndef _exti_hal_H
#define _exti_hal_H
#include "gd32f30x_libopt.h"
#include "gpio_hal.h"

//适配不同ic标准库
#ifndef GPIO_PUPD_NONE
#define GPIO_PUPD_NONE 0
#endif
#ifndef GPIO_PUPD_PULLUP
#define GPIO_PUPD_PULLUP 1
#endif
#ifndef GPIO_PUPD_PULLDOWN
#define GPIO_PUPD_PULLDOWN 2
#endif


class exti_hal:public gpio_hal
{
private:
    exti_line_enum linex;
    uint8_t get_exti_source(uint32_t gpio_periph);
    uint8_t get_exti_gpiopin(exti_line_enum linex);
public:
    exti_hal()=default;
    void init(exti_line_enum linex,                              //中断线
              uint32_t gpio_periph,                              //GPIO端口
              uint8_t nvic_irq_priority=16,                      //抢占优先级
              uint8_t nvic_irq_sub_priority=0,                   //响应优先级
              exti_mode_enum mode=EXTI_INTERRUPT,                //中断模式
              exti_trig_type_enum trig_type=EXTI_TRIG_FALLING,   //默认下降沿触发
              uint32_t pull_up_down=GPIO_PUPD_PULLUP             //默认无上下拉
              );
    void Callback_Register(void *str,void (*app)(void *str));
    void set_exti_trig(exti_trig_type_enum trig_type);
};





#endif
