#ifndef _usart_hal_H
#define _usart_hal_H
#include "gd32f30x_libopt.h"



class usart_hal
{
private:
    uint32_t usart_num;
public:
    enum struct usart_gpio_group{
        group0,
        group1,
        group2,
        group3
    };
    usart_hal()=default;
    void usart_init(uint32_t usart_periph,
               uint32_t baudrate=115200,
               usart_gpio_group gpio_group=usart_gpio_group::group0,
               uint32_t wlen=USART_WL_8BIT,
               uint32_t stblen=USART_STB_1BIT,
               uint32_t paritycfg=USART_PM_NONE,
               uint32_t rtsconfig=USART_RTS_DISABLE,
               uint32_t ctsconfig=USART_CTS_DISABLE,
               uint32_t txconfig=USART_TRANSMIT_ENABLE,
               uint32_t rxconfig=USART_RECEIVE_ENABLE,
               usart_interrupt_enum inttype=USART_INT_RBNE,
               uint8_t nvic_irq_priority=4
            );
    void write_packet(uint8_t *data,uint16_t num);
    void Callback_Register(void *str,void (*app)(uint8_t,void*));
};





#endif