#include "spi_hal.h"




void spi_hal::spi__init( uint32_t spi_periph,
                    spi_gpio_group gpio_group,
                    uint32_t device_mode,
                    uint32_t trans_mode,
                    uint32_t frame_size,
                    uint32_t nss,
                    uint32_t endian,
                    uint32_t clock_polarity_phase,
                    uint32_t prescale)
{
    this->spi_periph = spi_periph;
    this->spi_group = gpio_group;
    switch (this->spi_periph)
    {
        case SPI0:
            rcu_periph_clock_enable(RCU_SPI0);
            switch (this->spi_group)
            {
                case spi_gpio_group::group0:
                    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
                    break;
                case spi_gpio_group::group1:
                    gpio_pin_remap_config(GPIO_SPI0_REMAP,ENABLE);
                    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
                    break;
                default:
                    break;
            }
            break;
        case SPI1:
            rcu_periph_clock_enable(RCU_SPI1);
            switch (this->spi_group)
            {
                case spi_gpio_group::group0:
                    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
                    break;
                default:
                    break;
            }
            break;
        case SPI2:
            rcu_periph_clock_enable(RCU_SPI2);
            switch (this->spi_group)
            {
                case spi_gpio_group::group0:
                    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
                    break;
                case spi_gpio_group::group1:
                    gpio_pin_remap_config(GPIO_SPI2_REMAP,ENABLE);
                    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    spi_i2s_deinit(this->spi_periph);
    spi_parameter_struct spi_init_struct;
    spi_init_struct.device_mode = device_mode;
    spi_init_struct.trans_mode = trans_mode;
    spi_init_struct.frame_size = frame_size;
    spi_init_struct.nss = nss;
    spi_init_struct.endian = endian;
    spi_init_struct.clock_polarity_phase = clock_polarity_phase;
    spi_init_struct.prescale = prescale;
    spi_init(this->spi_periph,&spi_init_struct);
    //spi_enable(this->spi_periph);
}

uint8_t spi_hal::readwrite_byte(uint16_t data)
{
	while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(this->spi_periph,data);
	while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_RBNE)==RESET);
	return spi_i2s_data_receive(this->spi_periph);
}
void spi_hal::write(uint16_t data)
{
    while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(this->spi_periph,data);
}
uint8_t spi_hal::read(void)
{
    while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_TBE)==RESET);
	spi_i2s_data_transmit(this->spi_periph,0x00);
	while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_RBNE)==RESET);
	return spi_i2s_data_receive(this->spi_periph);
}
void spi_hal::readwrite_packet(uint8_t *txdata,uint8_t *rxdata,uint16_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_TBE)==RESET);
		spi_i2s_data_transmit(this->spi_periph,txdata[i]);
		while(spi_i2s_flag_get(this->spi_periph,SPI_FLAG_RBNE)==RESET);
		rxdata[i]=spi_i2s_data_receive(this->spi_periph);
	}
}

void spi_hal::frame_format_set(uint32_t frame_format)
{
    SPI_CTL0(this->spi_periph) &= (uint32_t)(~SPI_CTL0_FF16);
    SPI_CTL0(this->spi_periph) |= (uint32_t)frame_format;
}

void spi_hal::dma_write(uint16_t *txdata,uint32_t num)
{
    uint32_t tx_len,len_count;
    len_count=num;
    
    while(len_count)
    {
        while (this->spi_dma.dma_transfer_flag);//等待发送完成
        if(len_count>65534)
            tx_len=65535;
        else
            tx_len=len_count;
        len_count-=tx_len;
        this->spi_dma.channel_switch(DISABLE);//关闭dma通道
        this->spi_switch(DISABLE);
        this->spi_dma.memory_address_config((uint32_t)txdata);//设置内存地址
        this->spi_dma.transfer_number_config(tx_len);//设置发送数据量
        this->spi_dma.dma_transfer_flag = true;
        this->spi_switch(ENABLE);
        this->spi_dma.channel_switch(ENABLE);//打开dma通道
        

        //while(this->spi_dma.flag_get(DMA_FLAG_FTF)==RESET);
        //this->spi_dma.flag_clear(DMA_FLAG_FTF);
    }
}


void spi_hal::dma_switch(uint8_t dma,ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        if(SPI_DMA_TRANSMIT == dma){
            SPI_CTL1(this->spi_periph) |= (uint32_t)SPI_CTL1_DMATEN;
        }else{
            SPI_CTL1(this->spi_periph) |= (uint32_t)SPI_CTL1_DMAREN;
        }
        break;
    case DISABLE:
        if(SPI_DMA_TRANSMIT == dma){
            SPI_CTL1(this->spi_periph) &= (uint32_t)(~SPI_CTL1_DMATEN);
        }else{
            SPI_CTL1(this->spi_periph) &= (uint32_t)(~SPI_CTL1_DMAREN);
        }
        break;
    default:
        break;
    }

}
void spi_hal::spi_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        SPI_CTL0(this->spi_periph) |= (uint32_t)SPI_CTL0_SPIEN;
        break;
    case DISABLE:
        SPI_CTL0(this->spi_periph) &= (uint32_t)(~SPI_CTL0_SPIEN);
        break;
    default:
        break;
    }
}



void spi_hal::qspi_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        SPI_QCTL(this->spi_periph) |= (uint32_t)SPI_QCTL_QMOD;
        break;
    case DISABLE:
        SPI_QCTL(this->spi_periph) &= (uint32_t)(~SPI_QCTL_QMOD);
        break;
    default:
        break;
    }
    
}



void spi_hal::qspi_mode_set(qspi_mode value)
{
    switch (value)
    {
    case qspi_mode::read:
        SPI_QCTL(this->spi_periph) |= (uint32_t)SPI_QCTL_QRD;
        break;
    case qspi_mode::write:
        SPI_QCTL(this->spi_periph) &= (uint32_t)(~SPI_QCTL_QRD);
        break;    
    default:
        break;
    }
}


void spi_hal::qspi_io23_output_switch(ControlStatus value)
{
    switch (value)
    {
    case ENABLE:
        switch (this->spi_group)
        {
        case spi_gpio_group::group0 :
            gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2|GPIO_PIN_3);
            break;
        case spi_gpio_group::group1 :
            gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6|GPIO_PIN_7);
            break;
        default:
            break;
        }
        SPI_QCTL(spi_periph) |= (uint32_t)SPI_QCTL_IO23_DRV;
        break;
    case DISABLE:
        SPI_QCTL(spi_periph) &= (uint32_t)(~SPI_QCTL_IO23_DRV);
        break;
    default:
        break;
    }
}

void spi_hal::spi_bidirectional_transfer_config(uint32_t transfer_direction)
{
    switch (transfer_direction)
    {
    case SPI_BIDIRECTIONAL_TRANSMIT:
        SPI_CTL0(this->spi_periph) |= (uint32_t)SPI_BIDIRECTIONAL_TRANSMIT;
        break;
    case SPI_BIDIRECTIONAL_RECEIVE:
        SPI_CTL0(this->spi_periph) &= SPI_BIDIRECTIONAL_RECEIVE;
        break;
    default:
        break;
    }
}