#include "main.h"
#include "usart_hal.h"
#include "spi_hal.h"
#include "spi_hal.h"
#include "System_time.h"
#include "gpio_hal.h"
#include "RingBuffer.h"

#include "si24r1_slave_drv.h"
#include "si24r1_master_drv.h"
#include "button_drv.h"

usart_hal1 usart1__;
RingBuffer usart_buffer;

void si24r1_revice(uint8_t *data,uint8_t num)
{
    usart1__.write_packet(data,num);
}

void usart_master(uint8_t data,void *temp)
{
    ((si24r1_master_drv*)temp)->send_buffer.write(&data);
}
void usart_slave(uint8_t data,void *temp)
{
    ((si24r1_slave_drv*)temp)->send_buffer.write(&data);
}

void Main(void)
{
    static uint8_t recive_data[100],test_data[31]={
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        '\n'
    };
    static uint16_t recive_len,test_i=0;
    si24r1_master_drv si24r1_master_device;
    si24r1_slave_drv si24r1_slave_device;
    gpio_hal LEDG,LEDR;
    button_drv key;
    LEDG.init(GPIOB,GPIO_PIN_0);
    usart1__.init(USART1,115200);
    key.button_init(GPIOA,GPIO_PIN_4,button_drv::trigger_high);
    {
        si24r1_slave_device.si24r1_slave_init();
        while(1)
        {
            LEDG.high();
            if(key.Get_Button_Long_Press())
            {
                LEDG.low();
                si24r1_slave_device.si24r1_auto_matching();
            }
            recive_len = si24r1_slave_device.recive_buffer.read_packet(recive_data,100);
            usart1__.write_packet(recive_data,recive_len);
            delay_ms(5);
            si24r1_slave_device.send_buffer.write_packet(test_data,31);
            test_i++;
            if(test_i == 30)
            {
                test_i=0;
            }
            if(test_i == 0)
            {
                test_data[test_i]=0x5c;
                test_data[29]='0';
            }else
            {
                test_data[test_i]=0x5c;
                test_data[test_i-1]='0';
            }
        } 
    }
}


