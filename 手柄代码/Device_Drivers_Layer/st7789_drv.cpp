#include "st7789_drv.h"


void spi2_dma_backcall(void *str)
{
    ((st7789_drv*)(str))->st7789_spi.spi_dma.dma_transfer_flag = false;
    ((st7789_drv*)(str))->st7789_spi.frame_format_set(SPI_FRAMESIZE_8BIT);
    ((st7789_drv*)(str))->CS.high();
}


void st7789_drv::st7789_init(void)
{

    this->st7789_spi.spi_dma.dma_init(DMA1,
                                    DMA_CH1,
                                    (uint32_t)&SPI_DATA(SPI2),
                                    DMA_PERIPHERAL_WIDTH_16BIT,
                                    0x0000,
                                    DMA_MEMORY_WIDTH_16BIT,
                                    0,
                                    DMA_PRIORITY_HIGH,
                                    DMA_PERIPH_INCREASE_DISABLE,
                                    DMA_MEMORY_INCREASE_ENABLE,
                                    DMA_MEMORY_TO_PERIPHERAL,
                                    1);
    this->st7789_spi.spi_dma.circulation_switch(DISABLE);
    this->st7789_spi.spi_dma.memory_to_memory_switch(DISABLE); 
    this->st7789_spi.spi_dma.interrupt_switch(DMA_INT_FTF,ENABLE);
    this->st7789_spi.spi_dma.Callback_Register(this,spi2_dma_backcall);

    this->st7789_spi.spi__init(  SPI2,
                            spi_hal::spi_gpio_group::group0,
                            SPI_MASTER,
                            SPI_TRANSMODE_FULLDUPLEX,
                            SPI_FRAMESIZE_8BIT,
                            SPI_NSS_SOFT,
                            SPI_ENDIAN_MSB,
                            SPI_CK_PL_HIGH_PH_2EDGE,
                            SPI_PSC_2);

    this->st7789_spi.spi_switch(ENABLE);
    this->st7789_spi.spi_dma.channel_switch(ENABLE);
    this->st7789_spi.dma_switch(SPI_DMA_TRANSMIT,ENABLE);


    this->DC.gpio__init(GPIOD,GPIO_PIN_2);
    this->CS.gpio__init(GPIOA,GPIO_PIN_15);
    this->Backlight.pwm_init(TIMER3,10000,1000,timer_ch0);
    this->Reset.gpio__init(GPIOB,GPIO_PIN_4);
    this->width = 280;
    this->length = 240;

    this->Reset.low();
    delay_ms(100);
    this->Reset.high();
    delay_ms(120);
    this->write_cmd(0x11);//软件复位
    delay_ms(120);
    //屏幕方向
    this->write_cmd(0x36);
    this->write_data(0xA0);
    /* RGB 5-6-5-bit  */
    this->write_cmd(0x3A);
    this->write_data(0x05);
    /* Porch Setting */
    this->write_cmd(0xB2);
    this->write_data(0x0C);
    this->write_data(0x0C);
    this->write_data(0x00);
    this->write_data(0x33);
    this->write_data(0x33);
    /*  Gate Control */
    this->write_cmd(0xB7);
    this->write_data(0x35);
    /* VCOM Setting */
    this->write_cmd(0xBB);
    this->write_data(0x32);   //Vcom=1.625V

    /* VDV and VRH Command Enable */
    this->write_cmd(0xC2);
    this->write_data(0x01);

    /* VRH Set */
    this->write_cmd(0xC3);
    this->write_data(0x15);

    /* VDV Set */
    this->write_cmd(0xC4);
    this->write_data(0x20);

    /* Frame Rate Control in Normal Mode */
    this->write_cmd(0xC6);
    this->write_data(0x0F);	//60MHZ

    /* Power Control 1 */
    this->write_cmd(0xD0);
    this->write_data(0xA4);
    this->write_data(0xA1);

    /* Positive Voltage Gamma Control */
    this->write_cmd(0xE0);
    this->write_data(0xD0);
    this->write_data(0x08);
    this->write_data(0x0E);
    this->write_data(0x09);
    this->write_data(0x09);
    this->write_data(0x05);
    this->write_data(0x31);
    this->write_data(0x33);
    this->write_data(0x48);
    this->write_data(0x17);
    this->write_data(0x14);
    this->write_data(0x15);
    this->write_data(0x31);
    this->write_data(0x34);

    /* Negative Voltage Gamma Control */
    this->write_cmd(0xE1);
    this->write_data(0xD0);
    this->write_data(0x08);
    this->write_data(0x0E);
    this->write_data(0x09);
    this->write_data(0x09);
    this->write_data(0x15);
    this->write_data(0x31);
    this->write_data(0x33);
    this->write_data(0x48);
    this->write_data(0x17);
    this->write_data(0x14);
    this->write_data(0x15);
    this->write_data(0x31);
    this->write_data(0x34);
    /* 显示反转打开 */
    this->write_cmd(0x21);
    this->write_cmd(0x29);
    this->Backlight.set_channel1_duty(20);
}

void st7789_drv::write_cmd(uint8_t cmd)
{
    this->DC.low();
    this->CS.low();
    this->st7789_spi.readwrite_byte(cmd);
    this->CS.high();
}

void st7789_drv::write_data(uint16_t data)
{
    this->DC.high();
    this->CS.low();
    this->st7789_spi.readwrite_byte(data);
    this->CS.high();
}

void st7789_drv::Address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    this->write_cmd(0x2a);
    this->write_data(x1>>8);
    this->write_data(x1);
    this->write_data(x2>>8);
    this->write_data(x2);

    this->write_cmd(0x2b);
    this->write_data(y1>>8);
    this->write_data(y1);
    this->write_data(y2>>8);
    this->write_data(y2);

    this->write_cmd(0x2c);
}
static uint16_t color__;
void st7789_drv::Clear(uint16_t color)
{
    uint16_t i, j;
    color__ = color;
    this->Address_set(20,0,this->width+20,this->length-1);
    this->st7789_spi.frame_format_set(SPI_FRAMESIZE_16BIT);
    this->DC.high();
    this->CS.low();
    this->st7789_spi.spi_dma.memory_increase_switch(DISABLE);
    this->st7789_spi.dma_write(&color__,this->length*this->width-1);
    this->CS.high();
    this->st7789_spi.frame_format_set(SPI_FRAMESIZE_8BIT);
}



void st7789_drv::Show_image(uint16_t img_width,uint16_t img_length)
{
    uint16_t i, j;
    this->Address_set(0,20,img_width-1,img_length+20);
    this->st7789_spi.frame_format_set(SPI_FRAMESIZE_16BIT);

    this->DC.high();
    this->CS.low();
    this->CS.high();
    this->st7789_spi.frame_format_set(SPI_FRAMESIZE_8BIT);
}


void st7789_drv::Show(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const void* color)
{
    uint16_t i, j;
    uint32_t num = (xend - xsta+1) * (yend - ysta+1);
    while (this->st7789_spi.spi_dma.dma_transfer_flag);
    this->Address_set(xsta+20,ysta,xend+20,yend);
    this->st7789_spi.frame_format_set(SPI_FRAMESIZE_16BIT);
    this->DC.high();
    this->CS.low();
    this->st7789_spi.spi_dma.memory_increase_switch(ENABLE);
    this->st7789_spi.dma_write((uint16_t*)color,num);
    
}

void st7789_drv::Set_light(int8_t level)
{
    if(level>0 && level<=100)
        this->Backlight.set_channel1_duty((float)level);
}
