#ifndef _pwm_hal_H
#define _pwm_hal_H
#include "timer_hal.h"

#define timer_ch0 0x01
#define timer_ch1 0x02
#define timer_ch2 0x04
#define timer_ch3 0x08
class pwm_hal:public timer_hal
{
private:
    uint16_t cycle;
    uint16_t resolution;
    uint32_t timer_num;
public:

    enum pwm_gpio_group{
        group0,
        group1,
        group2,
        group3
    };
    pwm_hal()=default;
    void pwm_init(uint32_t timer_periph,
					uint16_t cycle,//周期
					uint32_t resolution,    //分辨率
                    uint8_t timer_chx,
                    pwm_gpio_group gpio_group = pwm_gpio_group::group0,
                    uint16_t ocnpolarity = TIMER_OC_POLARITY_HIGH,
                    uint16_t ocnidlestate = TIMER_OC_IDLE_STATE_LOW
                 );
    void set_channel1_duty(float duty);
    void set_channel2_duty(float duty);
    void set_channel3_duty(float duty);
    void set_channel4_duty(float duty);
};




#endif
