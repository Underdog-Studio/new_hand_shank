#ifndef _st7789_drv_H
#define _st7789_drv_H

#include "spi_hal.h"
#include "gpio_hal.h"
#include "System_time.h"
#include "pwm_hal.h"

class st7789_drv
{
private:
    
    gpio_hal DC;           //数据命令控制脚
    gpio_hal Reset;        //复位控制脚
    
    pwm_hal Backlight;    //背光控制脚
    //像素点长宽
    uint16_t length=10;
    uint16_t width=20;

    void write_cmd(uint8_t cmd);
    void write_data(uint16_t data);
public:
    st7789_drv()=default;
    spi_hal st7789_spi;
    gpio_hal CS;           
    void st7789_init(void);
    void Address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    //清屏函数
    void Clear(uint16_t);

    void Show(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const void* color);
    void Show_image(uint16_t img_width,uint16_t img_length);
    void Set_light(int8_t level);
};





#endif

