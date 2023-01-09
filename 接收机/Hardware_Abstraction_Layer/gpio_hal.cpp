
#include "gpio_hal.h"



void gpio_hal::init(uint32_t gpio_periph,
                    uint32_t pin,
                    uint32_t mode,
                    uint32_t pull_up_down,
                    uint8_t otype,
                    uint32_t speed)
{
    this->gpio_periph = gpio_periph;
    this->gpio_pin =pin;
    gpio_mode_set(this->gpio_periph,mode,pull_up_down,pin);//默认输出模式
	gpio_output_options_set(this->gpio_periph,otype,speed,pin);//推挽
}


void gpio_hal::high(void)
{
    GPIO_BOP(this->gpio_periph) = (uint32_t)this->gpio_pin;
}

void gpio_hal::low(void)
{
    GPIO_BC(this->gpio_periph) = (uint32_t)this->gpio_pin;
}
void gpio_hal::toggle(void)
{
    GPIO_TG(this->gpio_periph) = (uint32_t)this->gpio_pin;
}

FlagStatus gpio_hal::get(void)
{
    if((uint32_t)RESET != (GPIO_ISTAT(this->gpio_periph)&(this->gpio_pin))){
        return SET; 
    }else{
        return RESET;
    }
}



