#ifndef _dma_hal_H
#define _dma_hal_H
#include "gd32f30x_libopt.h"

class dma_hal
{
private:
    uint32_t dma_periph;
    dma_channel_enum dma_channel;

public:
    dma_hal()=default;
    bool dma_transfer_flag = false;
    bool dma_init(  uint32_t dma_periph,        /*!< dma端口 */
                dma_channel_enum channelx,   /*!< dma通道 */
                uint32_t periph_addr,       /*!< 外设基址 */
                uint32_t periph_width,      /*!< 外设的传输数据大小 */
                uint32_t memory_addr,       /*!< 内存地址 */
                uint32_t memory_width,      /*!< 传输数据的内存大小 */
                uint32_t number,            /*!< 通道数据传输数量 */
                uint32_t priority,          /*!< 通道优先级 */
                uint8_t periph_inc,         /*!< 外设增加模式 */
                uint8_t memory_inc,         /*!< 内存增加模式 */
                uint8_t direction,           /*!< 通道数据传输方向 */
                uint8_t nvic_irq_priority=255 //中断优先级
              );
    void reset(void);
    void circulation_switch(ControlStatus value);//设置dma循环模式
    void channel_switch(ControlStatus value);//dma通道开关
    void memory_to_memory_switch(ControlStatus value);//内存到内存开关
    void memory_increase_switch(ControlStatus value);//内存地址自增开关
    void periph_increase_switch(ControlStatus value);//外设地址自增开关
    void periph_address_config(uint32_t address);//外设地址配置
    void memory_address_config(uint32_t address);//内存地址配置
    void transfer_number_config(uint32_t number);//发送数量配置
    void priority_config(uint32_t priority);//优先级设置
    void memory_width_config(uint32_t mwidth);//内存宽度设置
    void periph_width_config(uint32_t pwidth);//外设宽度设置
    void transfer_direction_config(uint32_t direction);//数据传输方向配置
    FlagStatus flag_get(uint32_t flag);//获取dma标志位
    void flag_clear(uint32_t flag);//清除dma标志位
    FlagStatus interrupt_flag_get(uint32_t flag);//中断标志位获取
    void interrupt_flag_clear(uint32_t flag);//中断标志位清除
    void interrupt_switch(uint32_t source,ControlStatus value);//中断开关
    void Callback_Register(void *str,void (*app)(void*));//中断回调注册
};

#endif
