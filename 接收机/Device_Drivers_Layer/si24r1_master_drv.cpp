#include "si24r1_master_drv.h"

void si24r1_master_irq_callback(void *str);
void si24r1_master_timer_callback(void *str);


/*
*@Description :主机初始化
*/
void si24r1_master_drv::si24r1_master_init(void)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6];
    this->init();
    this->irq.Callback_Register(this,si24r1_master_irq_callback);
    this->irq.init(EXTI_0,GPIOA,2);//初始化irq中断线对象
    this->si24r1_timer.Callback_Register(this,si24r1_master_timer_callback);
    this->si24r1_timer.init(TIMER2,100,1000,3,TIMER_COUNTER_EDGE,TIMER_COUNTER_DOWN);
    //this->si24r1_timer.set_counter_value(3600-1);//设置定时器计数值
    test.init(GPIOB,GPIO_PIN_0);
    test.low();
    this->CE.low();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::SETUP_AW;
    spi_txbuff[1] = 0x03;//--5bit
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::TX_ADDR;
    memmove(spi_txbuff+1,this->IP,this->TX_ADR_WIDTH);
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->TX_ADR_WIDTH+1);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
    memmove(spi_txbuff+1,this->IP,5);
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::SETUP_RETR;
    spi_txbuff[1] = 0x1A;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::FEATURE;
    spi_txbuff[1] = 0x06;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::DYNPD;
    spi_txbuff[1] = 0x01;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
    spi_txbuff[1] = this->channel;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_SETUP;
    spi_txbuff[1] = 0x0F;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::CONFIG;
    spi_txbuff[1] = 0x0E;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    this->si24r1_timer.timer_switch(ENABLE);
}

void si24r1_master_drv::Send_data(void)
{
    uint8_t spi_txbuff[32],spi_rxbuff[32];
    this->CE.low();
    spi_txbuff[0] = si24r1_cmd::FLUSH_TX;
    spi_txbuff[1] = 0x00;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = this->send_buffer.read_packet(spi_txbuff+1,31);
    this->CS.low();
    spi_rxbuff[0] = this->si24r1_spi.readwrite_byte(si24r1_cmd::WR_TX_PLOAD);
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,spi_txbuff[0]+1);
    this->CS.high();
    this->CE.high();
}

/*
*@Description :irq中断回调
*@param       :控制指针
*/
void si24r1_master_irq_callback(void *str)
{
    uint8_t spi_txbuff[32],spi_rxbuff[32],state;
    (*(si24r1_master_drv*)str).CE.low();
    spi_txbuff[0] = si24r1_drv::si24r1_cmd::NRF_READ_REG + si24r1_drv::si24r1_register::STATUS;
    spi_txbuff[1] = 0x00; 
    ((si24r1_master_drv*)str)->CS.low();
    ((si24r1_master_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    ((si24r1_master_drv*)str)->CS.high();
    state = spi_rxbuff[1];
    spi_txbuff[0] = si24r1_drv::si24r1_cmd::NRF_WRITE_REG + si24r1_drv::si24r1_register::STATUS;
    spi_txbuff[1] = 0xff;
    ((si24r1_master_drv*)str)->CS.low();
    ((si24r1_master_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    ((si24r1_master_drv*)str)->CS.high();
    if(state & si24r1_drv::irq_state::MAX_TX)
    {
        spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_TX;
        spi_txbuff[1] = 0x00;
        ((si24r1_master_drv*)str)->CS.low();
        ((si24r1_master_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        ((si24r1_master_drv*)str)->CS.high();
    }else if(state & si24r1_drv::irq_state::TX_OK)
    {
        if(state & si24r1_drv::irq_state::RXFIFO_EMPTY)
        {
        }else
        {
            ((si24r1_master_drv*)str)->test.low();
            spi_txbuff[0] = si24r1_drv::si24r1_cmd::R_RX_PL_WID;
            spi_txbuff[1] = 0x00;
            (*(si24r1_master_drv*)str).CS.low();
            (*(si24r1_master_drv*)str).si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
            ((si24r1_master_drv*)str)->CS.high();
            (*(si24r1_master_drv*)str).Recive_Len = spi_rxbuff[1];
            (*(si24r1_master_drv*)str).CS.low();
            (*(si24r1_master_drv*)str).si24r1_spi.readwrite_byte(si24r1_drv::si24r1_cmd::RD_RX_PLOAD);
            (*(si24r1_master_drv*)str).si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,(*(si24r1_master_drv*)str).Recive_Len);
            ((si24r1_master_drv*)str)->CS.high();
            (*(si24r1_master_drv*)str).recive_buffer.write_packet(spi_rxbuff+1,spi_rxbuff[0]);
            spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
            spi_txbuff[1] = 0x00; 
            ((si24r1_master_drv*)str)->CS.low();
            ((si24r1_master_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
            ((si24r1_master_drv*)str)->CS.high();
            spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_TX;
            spi_txbuff[1] = 0x00; 
            ((si24r1_master_drv*)str)->CS.low();
            ((si24r1_master_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
            ((si24r1_master_drv*)str)->CS.high();
        }
    }
}

/*
*@Description :定时器中断回调函数
*/
void si24r1_master_timer_callback(void *str)
{
    ((si24r1_master_drv*)str)->Send_data();
    ((si24r1_master_drv*)str)->test.high();
}


