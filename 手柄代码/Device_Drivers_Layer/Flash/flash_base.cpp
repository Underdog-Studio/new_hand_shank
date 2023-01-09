#include "flash_base.h"



void flash_base::flash_init(void)
{
    this->GD25_QSPI.spi__init(  SPI0,
                                spi_hal::spi_gpio_group::group0,
                                SPI_MASTER,
                                SPI_TRANSMODE_FULLDUPLEX,
                                SPI_FRAMESIZE_8BIT,
                                SPI_NSS_SOFT,
                                SPI_ENDIAN_MSB,
                                SPI_CK_PL_HIGH_PH_2EDGE,
                                SPI_PSC_256);
     gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_2|GPIO_PIN_3);
     this->GD25_QSPI.spi_switch(ENABLE);
     this->GD25_CS.gpio__init(GPIOA,GPIO_PIN_4);
     this->GD25_CS.high();
}
/*
*@Description :读SR寄存器
*@param       :待读出SR寄存器值
*/
SR_reg flash_base::read_SR(void)
{
    uint8_t read_byte;
    this->GD25_CS.low();//CS拉低
    this->GD25_QSPI.readwrite_byte(cmd::ReadStatusReg1);
    read_byte = this->GD25_QSPI.readwrite_byte(cmd::Nop);
    this->GD25_CS.high();//CS拉高
    return *(SR_reg*)(&read_byte);
}
/*
*@Description :写SR寄存器
*@param       :待写入SR寄存器值
*/
void flash_base::write_SR(SR_reg SR)
{
    this->GD25_CS.low();//CS拉低
    this->GD25_QSPI.readwrite_byte(cmd::WriteStatusReg1);
    this->GD25_QSPI.readwrite_byte(*(uint8_t*)(&SR));
    this->GD25_CS.high();//CS拉高
}
/*
*@Description :写功能开关
*@param       :ENABLE -- DISABLE
*/
void flash_base::write_switch(bool value)
{
    switch (value)
    {
    case true:
        this->GD25_CS.low();//CS拉低
        this->GD25_QSPI.readwrite_byte(cmd::WriteEnable);
        this->GD25_CS.high();//CS拉高
        break;
    case false:
        this->GD25_CS.low();//CS拉低
        this->GD25_QSPI.readwrite_byte(cmd::WriteDisable);
        this->GD25_CS.high();//CS拉高
        break;
    default:
        break;
    }
}
/*
*@Description :读设备ID
*@return      :设备ID
*/
uint16_t flash_base::readID(void)
{
    uint16_t read_byte=0;
    this->GD25_CS.low();//CS拉低
    this->GD25_QSPI.readwrite_byte(cmd::ManufactDeviceID);
    this->GD25_QSPI.readwrite_byte(0x00);
    this->GD25_QSPI.readwrite_byte(0x00);
    this->GD25_QSPI.readwrite_byte(0x00);
    read_byte |= this->GD25_QSPI.readwrite_byte(cmd::Nop)<<8;
    read_byte |= this->GD25_QSPI.readwrite_byte(cmd::Nop);
    this->GD25_CS.high();//CS拉高
    return read_byte;
}
/*
*@Description :擦除一个扇区
*@param       :待擦除扇区地址 4K对齐
*/
void flash_base::erase_sector(uint32_t addr)
{
    this->wait_busy();
    this->GD25_CS.low();
    this->GD25_QSPI.readwrite_byte(cmd::SectorErase);//擦除对应扇区
    this->GD25_QSPI.readwrite_byte((uint8_t)(addr>>16));
    this->GD25_QSPI.readwrite_byte((uint8_t)(addr>>8));
    this->GD25_QSPI.readwrite_byte((uint8_t)(addr));
    this->GD25_CS.high();
}

void flash_base::erase_chip(void)
{
    this->wait_busy();
    this->GD25_CS.low();
    this->GD25_QSPI.readwrite_byte(cmd::ChipErase);//擦除对应扇区
    this->GD25_CS.high();
    this->wait_busy();
}

/*
*@Description :写入数据
*@param       :待写入数据指针
*@param       :写入地址
*@param       :写入数量
*/
volatile uint8_t data_show[4096];
void flash_base::write_sector(uint8_t *write_buffer,uint32_t write_addr)
{
    for(uint16_t i=0;i<4096;i++)
        data_show[i] = write_buffer[i];

    if(write_addr%4096 == 0)//检测地址是否4k对齐
    {
        this->erase_sector(write_addr);
        for(uint8_t i=0;i<16;i++)//16页
        {
            this->write_page((write_buffer+i*256),write_addr+i*256,256);//写入一页256字节
        }

    }
}

/*
*@Description :写入一页数据 --请确保已打开写使能
*@param       :待写入数据指针
*@param       :写入地址
*@param       :写入数量 <=256
*/
void flash_base::write_page(uint8_t *write_buffer,uint32_t write_addr,uint16_t write_len)
{
    this->wait_busy();//等待写入结束
    this->write_switch(true);//打开写使能
    this->GD25_CS.low();
    this->GD25_QSPI.readwrite_byte(cmd::PageProgram);
    this->GD25_QSPI.readwrite_byte((uint8_t)(write_addr>>16));
    this->GD25_QSPI.readwrite_byte((uint8_t)(write_addr>>8));
    this->GD25_QSPI.readwrite_byte((uint8_t)(write_addr));
    for(uint16_t i=0;i<write_len;i++)
        this->GD25_QSPI.readwrite_byte(write_buffer[i]);
    this->GD25_CS.high();
    //this->write_switch(false);//关闭写使能
    delay_ms(2);
}

/*
*@Description :读数据
*@param       :存储读出数据指针
*@param       :读出地址
*@param       :读出数量
*/
void flash_base::read_data(uint8_t *read_buffer,uint32_t read_addr,uint32_t read_len)
{
    this->wait_busy();
    this->GD25_CS.low();
    this->GD25_QSPI.readwrite_byte(cmd::ReadData);
    this->GD25_QSPI.readwrite_byte((uint8_t)(read_addr>>16));
    this->GD25_QSPI.readwrite_byte((uint8_t)(read_addr>>8));
    this->GD25_QSPI.readwrite_byte((uint8_t)(read_addr));
    for (uint32_t i = 0; i < read_len; i++)
    {
        read_buffer[i] = this->GD25_QSPI.readwrite_byte(0x00);
    }
    this->GD25_CS.high();
}

/*
*@Description :等待FLASH空闲
*/
void flash_base::wait_busy(void)
{
    while (this->read_SR().BUSY); 
}