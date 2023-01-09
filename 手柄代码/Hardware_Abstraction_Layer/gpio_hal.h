#ifndef _gpio_hal_H
#define _gpio_hal_H
#include "gd32f30x_libopt.h"

class gpio_hal
{
private:
    uint32_t gpio_periph;
    uint32_t gpio_pin;
    uint16_t *analog_value;
public:
    gpio_hal()=default;
    void gpio__init(uint32_t gpio_periph,
              uint32_t pin,
              uint32_t mode=GPIO_MODE_OUT_PP,
              uint32_t speed=GPIO_OSPEED_50MHZ
              );

    void high(void);
    void low(void);
    void port_write(uint8_t);
    uint8_t port_read(void);
    void toggle(void);
    FlagStatus get(void);//Digital
    void set_analog_addr(uint16_t *addr);
    uint16_t get_analog(void);
};



#endif



