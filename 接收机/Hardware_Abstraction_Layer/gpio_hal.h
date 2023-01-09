#ifndef _gpio_hal_H
#define _gpio_hal_H
#include "gd32e23x_it.h"
#include "gd32e23x_gpio.h"

class gpio_hal
{
private:
    uint8_t data_ = 1;
    uint32_t gpio_periph;
    uint32_t gpio_pin;
public:
    gpio_hal()=default;
    void init(uint32_t gpio_periph,
              uint32_t pin,
              uint32_t mode=GPIO_MODE_OUTPUT,
              uint32_t pull_up_down=GPIO_PUPD_NONE,
              uint8_t otype=GPIO_OTYPE_PP,
              uint32_t speed=GPIO_OSPEED_50MHZ
              );

    void high(void);
    void low(void);
    void toggle(void);
    FlagStatus get(void);
};




#endif



