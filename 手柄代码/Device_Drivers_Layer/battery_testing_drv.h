#ifndef _battery_testing_drv_H
#define _battery_testing_drv_H
#include "gpio_hal.h"
#include "adc_hal.h"



class battery_testing_drv
{
    typedef enum
    {
        Charging,      //充电中
        bat_full,      //电量充满
        bat_plenty,    //电量充足
        bat_lack,      //电量不足
        bat_urgent,    //电量报警
    }battery_state;
private:
    float bat_level=0;//电池电量 0-100
    uint16_t full_voltage=3830;//满电电压
    uint16_t zero_voltage=2462;//0电压
    battery_state bat_state;//电池状态
    gpio_hal Charging_testing,bat_full_testing;
    adc_hal bat_level_testing;
public:
    battery_testing_drv()=default;
    void battery_testing_init(void);
    void battery_testing_tick(void);
    float get_batter_level(void);
};





#endif

