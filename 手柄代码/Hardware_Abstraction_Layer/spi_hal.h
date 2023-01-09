#ifndef _spi_hal_H
#define _spi_hal_H

#include "gd32f30x_libopt.h"
#include "dma_hal.h"
#include "System_time.h"
class spi_hal
{


public:
    spi_hal()=default;
    enum struct spi_gpio_group{
        group0,
        group1,
        group2,
        group3
    };
    

    enum struct qspi_mode{
        read,
        write,
    };

    void spi__init(  uint32_t spi_periph,
                spi_gpio_group gpio_group=spi_gpio_group::group0,
                uint32_t device_mode=SPI_MASTER,
                uint32_t trans_mode=SPI_TRANSMODE_FULLDUPLEX,
                uint32_t frame_size=SPI_FRAMESIZE_8BIT,
                uint32_t nss=SPI_NSS_SOFT,
                uint32_t endian=SPI_ENDIAN_MSB,
                uint32_t clock_polarity_phase=SPI_CK_PL_LOW_PH_1EDGE,
                uint32_t prescale=SPI_PSC_8
             );
    dma_hal spi_dma;
    uint8_t readwrite_byte(uint16_t data);
    void write(uint16_t data);
    uint8_t read(void);
    void readwrite_packet(uint8_t *txdata,uint8_t *rxdata,uint16_t num);
    void dma_write(uint16_t *txdata,uint32_t num);
    void Callback_Register(void (*app)(uint8_t));
    void frame_format_set(uint32_t frame_format);
    void dma_switch(uint8_t dma,ControlStatus value);
    void spi_switch(ControlStatus value);

    //QSPI
    void qspi_switch(ControlStatus value);
    void qspi_mode_set(qspi_mode value);
    void qspi_io23_output_switch(ControlStatus value);

    void spi_bidirectional_transfer_config(uint32_t transfer_direction);
private:
    uint32_t spi_periph;
    spi_gpio_group spi_group;
};




#endif
