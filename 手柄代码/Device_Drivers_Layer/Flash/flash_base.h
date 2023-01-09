#ifndef _flash_base_H
#define _flash_base_H


#include "spi_hal.h"
#include "gpio_hal.h"

typedef struct 
{
    uint8_t BUSY:1;
    uint8_t WEL :1;
    uint8_t BP0 :1;
    uint8_t BP1 :1;
    uint8_t BP2 :1;
    uint8_t TB  :1;
    uint8_t RV  :1;
    uint8_t SPR :1;
}SR_reg;



class flash_base
{
private:
    spi_hal GD25_QSPI;
    gpio_hal GD25_CS;
public:
    enum cmd
    {
        WriteEnable      = 0x06,
        WriteDisable     = 0x04,
        ReadStatusReg1   = 0x05,
        WriteStatusReg1  = 0x01,
        ReadStatusReg2   = 0x35,
        WriteStatusReg2  = 0x31,
        ReadData		 = 0x03,
        FastReadData	 = 0x0B,
        FastReadDual	 = 0x3B,
        PageProgram		 = 0x02,
        BlockErase		 = 0xD8,
        SectorErase		 = 0x20,
        ChipErase		 = 0xC7,
        PowerDown		 = 0xB9,
        ReleasePowerDown = 0xAB,
        DeviceID	     = 0xAB, 
        ManufactDeviceID = 0x90,
        JedecDeviceID    = 0x9F,
        Nop              = 0xff
    };
    flash_base()=default;
    void flash_init(void);
    //操作flash状态寄存器
    SR_reg read_SR(void);
    void write_SR(SR_reg SR);
    //flash 写开关
    void write_switch(bool value);
    //读取flash--ID
    uint16_t readID(void);

    //等待BUSY清空 flash空闲
    void wait_busy(void);
    //全片擦除
    void erase_chip(void);
    //擦除一个扇区
    void erase_sector(uint32_t addr);
    //写入一个扇区
    void write_sector(uint8_t *write_buffer,uint32_t write_addr);
    //写入一个页
    void write_page(uint8_t *write_buffer,uint32_t write_addr,uint16_t write_len);
    //读任意地址数据
    void read_data(uint8_t *read_buffer,uint32_t read_addr,uint32_t read_len);
};




#endif
