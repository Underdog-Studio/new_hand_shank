#include "spi_hal.h"


void spi_hal::init( uint32_t spi_periph,
                    spi_gpio_group gpio_group,
                    uint32_t device_mode,
                    uint32_t trans_mode,
                    uint32_t frame_size,
                    uint32_t nss,
                    uint32_t endian,
                    uint32_t clock_polarity_phase,
                    uint32_t prescale)
{
    this->spi_num = spi_periph;
    switch (this->spi_num)
    {
    case SPI0:
        rcu_periph_clock_enable(RCU_SPI0);
        switch (gpio_group)
        {
            case spi_gpio_group::group0:
                gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_5);
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
                break;
            case spi_gpio_group::group1:
                gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
                break;
            case spi_gpio_group::group2:
                gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_13);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14|GPIO_PIN_15);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
                break;
            default:
                break;
        }
        break;
    case SPI1:
        rcu_periph_clock_enable(RCU_SPI1);
        switch (gpio_group)
        {
            case spi_gpio_group::group0:
                gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_1);
                gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_13|GPIO_PIN_14);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_1);
                gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_13|GPIO_PIN_14);
                gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
                gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14);
                break;
            case spi_gpio_group::group1:
                gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_13);
                gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14|GPIO_PIN_15);
                gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
                break;
            default:
                break;
        }
        break;
    default:
        break;
    }

    spi_i2s_deinit(this->spi_num);
    spi_parameter_struct spi_init_struct;
    spi_init_struct.device_mode = device_mode;
    spi_init_struct.trans_mode = trans_mode;
    spi_init_struct.frame_size = frame_size;
    spi_init_struct.nss = nss;
    spi_init_struct.endian = endian;
    spi_init_struct.clock_polarity_phase = clock_polarity_phase;
    spi_init_struct.prescale = prescale;
    spi_init(this->spi_num,&spi_init_struct);
    spi_fifo_access_size_config(this->spi_num,SPI_BYTE_ACCESS);//使能数据位
    spi_enable(this->spi_num);
}

uint8_t spi_hal::readwrite_byte(uint8_t data)
{
	while(spi_i2s_flag_get(this->spi_num,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(this->spi_num,data);
	while(spi_i2s_flag_get(this->spi_num,SPI_FLAG_RBNE)==RESET);
	return spi_i2s_data_receive(this->spi_num);

}

void spi_hal::readwrite_packet(uint8_t *txdata,uint8_t *rxdata,uint16_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		while(spi_i2s_flag_get(this->spi_num,SPI_FLAG_TBE)==RESET);
		spi_i2s_data_transmit(this->spi_num,txdata[i]);
		while(spi_i2s_flag_get(this->spi_num,SPI_FLAG_RBNE)==RESET);
		rxdata[i]=spi_i2s_data_receive(this->spi_num);
	}
}





