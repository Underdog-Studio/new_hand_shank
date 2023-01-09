#ifndef adc_hal_H
#define adc_hal_H
#include "gd32f30x_libopt.h"
#include "gpio_hal.h"
#include "dma_hal.h"

class adc_hal:public dma_hal,public gpio_hal
{
private:
    uint32_t adc_periph;
    uint32_t adc_channel;
    uint8_t adc_rank;
public:
    adc_hal()=default;
    void adc_init(uint32_t adcx,uint8_t channel_len);
    //adc通道初始化
    void adc_channel_init(uint8_t adc_channel);
    void adc_reset(void);
    //ADC开关
    void adc_switch(ControlStatus newvalue);
    //ADC模式配置
    void adc_mode_config(uint32_t mode);
    //ADC特殊功能配置
    void adc_special_function_config(uint32_t function , ControlStatus newvalue);
    //数据对齐配置
    void adc_data_alignment_config(uint32_t data_alignment);
    //通道长度配置
    void adc_channel_length_config(uint8_t adc_channel_group, uint32_t length);
    //常规通道配置
    void adc_regular_channel_config(uint8_t rank , uint8_t adc_channel , uint32_t sample_time);
    //外部触发配置
    void adc_external_trigger_config(uint8_t adc_channel_group, ControlStatus newvalue);
    //外部触发源配置
    void adc_external_trigger_source_config(uint8_t adc_channel_group, uint32_t external_trigger_source);
    //adc校准
    void adc_calibration(void);
    //DMA模式开关
    void adc_dma_mode_switch(ControlStatus newvalue);
    //打开软件触发
    void adc_software_trigger_enable(uint8_t adc_channel_group);
    //设置ADC分辨率
    void adc_resolution_config(uint32_t resolution);
    //获取adc通道数据
    uint16_t adc_get_channel_value(void);
};





#endif
