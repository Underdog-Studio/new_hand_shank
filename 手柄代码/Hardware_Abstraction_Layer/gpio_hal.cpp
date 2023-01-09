#include "gpio_hal.h"

void gpio_hal::gpio__init(uint32_t gpio_periph,
                    uint32_t pin,
                    uint32_t mode,
                    uint32_t speed)
{
    this->gpio_periph = gpio_periph;
    this->gpio_pin =pin;
    if(mode==GPIO_MODE_AIN)
    {

    }
    gpio_init(this->gpio_periph,mode,speed,pin);//默认输出模式
}

void gpio_hal::high(void)
{
    GPIO_BOP(this->gpio_periph) = (uint32_t)this->gpio_pin;
}

void gpio_hal::low(void)
{
    GPIO_BC(this->gpio_periph) = (uint32_t)this->gpio_pin;
}

void gpio_hal::port_write(uint8_t data)
{
    GPIO_OCTL(this->gpio_periph) = (uint32_t)data;
}


uint8_t gpio_hal::port_read(void)
{
    return (uint16_t)(GPIO_ISTAT(this->gpio_periph));
}

void gpio_hal::toggle(void)
{
	if((uint32_t)RESET !=(GPIO_OCTL(this->gpio_periph)&(this->gpio_pin))){
			GPIO_BC(this->gpio_periph) = (uint32_t)this->gpio_pin;//reset
	}else{
			GPIO_BOP(this->gpio_periph) = (uint32_t)this->gpio_pin;//set
	}
}

FlagStatus gpio_hal::get(void)
{
    if((uint32_t)RESET != (GPIO_ISTAT(this->gpio_periph)&(this->gpio_pin))){
        return SET; 
    }else{
        return RESET;
    }
}

uint16_t gpio_hal::get_analog(void)
{
    return *(this->analog_value);
}


void gpio_hal::set_analog_addr(uint16_t *addr)
{
    this->analog_value = addr;
}
