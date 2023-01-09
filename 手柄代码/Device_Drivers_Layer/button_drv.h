#ifndef _button_H
#define _button_H
#include "exti_hal.h"
#include "System_time.h"
class button_drv:public exti_hal
{
private:
    
public:
    typedef enum
    {
        trigger_high=0xff,
        trigger_low=0x00,
    }trigger_levels;
    button_drv()=default;
    void button_init(   uint32_t gpio_periph,
              		    uint32_t pin,
                        trigger_levels trigger_level=trigger_low//触发电平 默认低电平触发
                        );
    bool Button_Click = false,Button_Double_Click = false,Button_Long_Press = false;//单击双击长按标志位
    bool Button_State = false;//按键状态
    trigger_levels trigger_level;
    Time Button_T;
    uint8_t Button_Count=0;//按键计数值

    //获取按键单击
    bool Get_Button_Click(void);
    //获取按键双击
    bool Get_Button_Double_Click(void);
    //获取按键长按
    bool Get_Button_Long_Press(void);

};





#endif
