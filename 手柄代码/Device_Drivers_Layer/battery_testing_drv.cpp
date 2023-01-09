#include "battery_testing_drv.h"


void battery_testing_drv::battery_testing_init(void)
{
    this->bat_full_testing.gpio__init(GPIOB,GPIO_PIN_1,GPIO_MODE_IPU);
    this->Charging_testing.gpio__init(GPIOB,GPIO_PIN_2,GPIO_MODE_IPU);
    this->bat_level_testing.adc_init(ADC0,ADC_CHANNEL_1);



}

float battery_testing_drv::get_batter_level(void)
{
    return this->bat_level;
}

void battery_testing_drv::battery_testing_tick(void)
{
    this->bat_level = 100.0f/(float)(this->full_voltage - this->zero_voltage) * (this->bat_level_testing.adc_get_channel_value() - this->zero_voltage);
}
