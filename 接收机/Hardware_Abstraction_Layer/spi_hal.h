#ifndef _spi_hal_H
#define _spi_hal_H
#include "gd32e23x_it.h"
#include "gd32e23x_spi.h"



class spi_hal
{
private:
    uint32_t spi_num;
public:
    enum struct spi_gpio_group{
        group0,
        group1,
        group2,
        group3
    };
    spi_hal()=default;

    void init(  uint32_t spi_periph,
                spi_gpio_group gpio_group=spi_gpio_group::group0,
                uint32_t device_mode=SPI_MASTER,
                uint32_t trans_mode=SPI_TRANSMODE_FULLDUPLEX,
                uint32_t frame_size=SPI_FRAMESIZE_8BIT,
                uint32_t nss=SPI_NSS_SOFT,
                uint32_t endian=SPI_ENDIAN_MSB,
                uint32_t clock_polarity_phase=SPI_CK_PL_LOW_PH_1EDGE,
                uint32_t prescale=SPI_PSC_8
             );
    uint8_t readwrite_byte(uint8_t data);
    void readwrite_packet(uint8_t *txdata,uint8_t *rxdata,uint16_t num);
    void Callback_Register(void (*app)(uint8_t));
};



#endif


