#include "si24r1_drv.h"



void si24r1_drv::init(void)
{
    uint8_t spi_txbuff[2],spi_rxbuff[2];
    this->si24r1_spi.init(SPI0);
    this->CE.init(GPIOB,GPIO_PIN_5);
    this->CE.low();
    this->CS.init(GPIOB,GPIO_PIN_3);
    this->CS.high();
    this->recive_buffer.init(recive_data,512);
    this->send_buffer.init(send_data,512);

    delay_ms(100);


        spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::STATUS;
        spi_txbuff[1] = 0x7f;
        this->CS.low();
        this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        this->CS.high();
        delay_ms(3);    

}

