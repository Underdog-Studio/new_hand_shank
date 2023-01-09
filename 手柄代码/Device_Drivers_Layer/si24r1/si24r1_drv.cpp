#include "si24r1_drv.h"



void si24r1_drv::init(void)
{
    uint8_t spi_txbuff[2],spi_rxbuff[2];
    //初始化spi对象
    this->si24r1_spi.spi__init( SPI1,
                                spi_hal::spi_gpio_group::group0,
                                SPI_MASTER,
                                SPI_TRANSMODE_FULLDUPLEX,
                                SPI_FRAMESIZE_8BIT,
                                SPI_NSS_SOFT,
                                SPI_ENDIAN_MSB,
                                SPI_CK_PL_LOW_PH_1EDGE,
                                SPI_PSC_8);
    this->si24r1_spi.spi_switch(ENABLE);                     
    //初始化CE CS控制引脚对象
    this->CE.gpio__init(GPIOC,GPIO_PIN_12);
    this->CE.low();
    this->CS.gpio__init(GPIOC,GPIO_PIN_11);
    this->CS.high();
    //初始化接收发送缓冲区
    this->recive_buffer.init(recive_data,512);
    this->send_buffer.init(send_data,512);

    delay_ms(100);
    //进入关机模式
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::CONFIG;
    spi_txbuff[1] = 0x7D;//关机模式
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();

    //清除所有中断
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::STATUS;
    spi_txbuff[1] = 0x7f;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    delay_ms(3);    
}

