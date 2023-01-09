#include "si24r1_slave_drv.h"

static void si24r1_slave_irq_callback(void *str);
void si24r1_slave_timer_callback(void *str);


void si24r1_slave_drv::si24r1_slave_init(void)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6];
    this->init();
    this->irq.init(EXTI_0,GPIOA,1);
    this->irq.Callback_Register(this,si24r1_slave_irq_callback);
    this->text.init(GPIOB,GPIO_PIN_0);
    this->si24r1_timer.init(TIMER2,33,1000,2,TIMER_COUNTER_EDGE,TIMER_COUNTER_DOWN);
    this->si24r1_timer.Callback_Register(this,si24r1_slave_timer_callback);
    this->CE.low();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
    memmove(spi_txbuff+1,this->IP,this->RX_ADR_WIDTH);
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_PW_P0;
    spi_txbuff[1] = this->RX_ADR_WIDTH;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::EN_RXADDR;
    spi_txbuff[1] = 0x01;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::FEATURE;
    spi_txbuff[1] = 0x06;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::SETUP_AW;
    spi_txbuff[1] = 0x03;
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
    spi_txbuff[1] = 0x07;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::CONFIG;
    spi_txbuff[1] = 0x0F;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::STATUS;
    spi_txbuff[1] = 0x7f;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
    spi_txbuff[1] = 0x00; 
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = this->send_buffer.read_packet(spi_txbuff+1,31);
    this->CS.low();
    this->si24r1_spi.readwrite_byte(si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD);
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,spi_txbuff[0]+1);
    this->CS.high(); 
    this->CE.high();
}

/*
*@Description :irq中断回调
*@param       :控制指针
*/
static void si24r1_slave_irq_callback(void *str)
{
    uint8_t spi_txbuff[32],spi_rxbuff[32],state;

    (*(si24r1_slave_drv*)str).CE.low();

    spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::NRF_READ_REG + si24r1_slave_drv::si24r1_register::STATUS;
    spi_txbuff[1] = 0x00;
    ((si24r1_slave_drv*)str)->CS.low();
    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    ((si24r1_slave_drv*)str)->CS.high();
    state = spi_rxbuff[1];

    spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::NRF_WRITE_REG + si24r1_slave_drv::si24r1_register::STATUS;
    spi_txbuff[1] = state;
    ((si24r1_slave_drv*)str)->CS.low();
    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    ((si24r1_slave_drv*)str)->CS.high();

    if(((state & si24r1_slave_drv::irq_state::TX_OK) || (state & si24r1_slave_drv::irq_state::RX_OK)) && 
        (state&si24r1_slave_drv::irq_state::RXFIFO_EMPTY)==0x00)
    {
        
        spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::R_RX_PL_WID;
                spi_txbuff[1] = si24r1_slave_drv::si24r1_cmd::NOP;
        ((si24r1_slave_drv*)str)->CS.low();
        ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        ((si24r1_slave_drv*)str)->CS.high();
        (*(si24r1_slave_drv*)str).Recive_Len = spi_rxbuff[1];

        ((si24r1_slave_drv*)str)->CS.low();
        (*(si24r1_slave_drv*)str).si24r1_spi.readwrite_byte(si24r1_slave_drv::si24r1_cmd::RD_RX_PLOAD);
        (*(si24r1_slave_drv*)str).si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,(*(si24r1_slave_drv*)str).Recive_Len);
        ((si24r1_slave_drv*)str)->CS.high();
        
        switch (spi_rxbuff[0])
        {
            case 0x00:
                (*(si24r1_slave_drv*)str).recive_buffer.write_packet(spi_rxbuff+1,(*(si24r1_slave_drv*)str).Recive_Len-1);
                spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
                spi_txbuff[1] = 0x00; 
                ((si24r1_slave_drv*)str)->CS.low();
                ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                ((si24r1_slave_drv*)str)->CS.high();
                spi_txbuff[0] = 0x00;
                ((si24r1_slave_drv*)str)->Send_Len = ((si24r1_slave_drv*)str)->send_buffer.read_packet(spi_txbuff+1,31);
                ((si24r1_slave_drv*)str)->CS.low();
                ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_byte(si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD);
                ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,((si24r1_slave_drv*)str)->Send_Len+1);
                ((si24r1_slave_drv*)str)->CS.high(); 
                break;
            case 0x0f:
                if(((si24r1_slave_drv*)str)->si24r1_toggle_state == false)
                {
                    ((si24r1_slave_drv*)str)->toggle_channel = spi_rxbuff[1];
                    spi_txbuff[0] = si24r1_drv::si24r1_cmd::NRF_WRITE_REG + si24r1_drv::si24r1_register::RF_CH;
                    spi_txbuff[1] = spi_rxbuff[1];
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    ((si24r1_slave_drv*)str)->CS.high();
                    spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
                    spi_txbuff[1] = 0x00; 
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    ((si24r1_slave_drv*)str)->CS.high();
                    spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD;
                    spi_txbuff[1] = 0x0f;
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    ((si24r1_slave_drv*)str)->CS.high();
                    ((si24r1_slave_drv*)str)->si24r1_toggle_state = true;
                    ((si24r1_slave_drv*)str)->si24r1_timer.timer_switch(ENABLE);
                }else{
                    ((si24r1_slave_drv*)str)->channel = ((si24r1_slave_drv*)str)->toggle_channel;
                    ((si24r1_slave_drv*)str)->si24r1_toggle_state = false;
                    spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
                    spi_txbuff[1] = 0x00; 
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    ((si24r1_slave_drv*)str)->CS.high();
                    spi_txbuff[0] = 0x00;
                    ((si24r1_slave_drv*)str)->Send_Len = ((si24r1_slave_drv*)str)->send_buffer.read_packet(spi_txbuff+1,31);
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_byte(si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD);
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,((si24r1_slave_drv*)str)->Send_Len+1);
                    ((si24r1_slave_drv*)str)->CS.high(); 
                }
                break;
            case 0xf0:
                if(((si24r1_slave_drv*)str)->auto_matching_flag)
                {
                    memmove(((si24r1_slave_drv*)str)->IP,spi_rxbuff+1,((si24r1_slave_drv*)str)->RX_ADR_WIDTH);

                    
                    spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD;
                    spi_txbuff[1] = 0xf0;
                    spi_txbuff[2] = ((si24r1_slave_drv*)str)->auto_matching_freq;
                    ((si24r1_slave_drv*)str)->CS.low();
                    ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,3);
                    ((si24r1_slave_drv*)str)->CS.high(); 
                    ((si24r1_slave_drv*)str)->si24r1_timer.set_autoreload_value(6000);
                    ((si24r1_slave_drv*)str)->si24r1_timer.set_counter_value(6000);
                    ((si24r1_slave_drv*)str)->si24r1_timer.timer_switch(ENABLE);
                }
                break;
            default:
                break;
        }
        ((si24r1_slave_drv*)str)->CE.high();
    }
    
}


/*
*@Description :定时器中断回调
*@param       :控制指针
*/
void si24r1_slave_timer_callback(void *str)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6];
    ((si24r1_slave_drv*)str)->CE.low();
    ((si24r1_slave_drv*)str)->si24r1_timer.timer_switch(DISABLE);
    if(((si24r1_slave_drv*)str)->si24r1_toggle_state == true)
    {
        spi_txbuff[0] = si24r1_drv::si24r1_cmd::NRF_WRITE_REG + si24r1_drv::si24r1_register::RF_CH;
        spi_txbuff[1] = ((si24r1_slave_drv*)str)->channel;
        ((si24r1_slave_drv*)str)->CS.low();
        ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        ((si24r1_slave_drv*)str)->CS.high(); 
    }
    if(((si24r1_slave_drv*)str)->auto_matching_flag == true)
    {
        ((si24r1_slave_drv*)str)->channel = ((si24r1_slave_drv*)str)->auto_matching_freq;
        spi_txbuff[0] = si24r1_drv::si24r1_cmd::NRF_WRITE_REG + si24r1_drv::si24r1_register::RX_ADDR_P0;
        memmove(spi_txbuff+1,((si24r1_slave_drv*)str)->IP,((si24r1_slave_drv*)str)->RX_ADR_WIDTH);
        ((si24r1_slave_drv*)str)->CS.low();
        ((si24r1_slave_drv*)str)->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,((si24r1_slave_drv*)str)->RX_ADR_WIDTH+1);
        ((si24r1_slave_drv*)str)->CS.high();
        ((si24r1_slave_drv*)str)->auto_matching_flag = false;
    }
    ((si24r1_slave_drv*)str)->CE.high();
}


bool si24r1_slave_drv::si24r1_auto_matching(void)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6],Last_IP[5];
    memmove(Last_IP,this->IP,this->RX_ADR_WIDTH);
    this->CE.low();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
    memmove(spi_txbuff+1,this->auto_matching_IP,this->RX_ADR_WIDTH);
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_PW_P0;
    spi_txbuff[1] = this->RX_ADR_WIDTH;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::EN_RXADDR;
    spi_txbuff[1] = 0x01;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::FEATURE;
    spi_txbuff[1] = 0x06;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::SETUP_AW;
    spi_txbuff[1] = 0x03;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::DYNPD;
    spi_txbuff[1] = 0x01;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    this->auto_matching_freq = ((int)(Get_System_T().us/15.857)*2);
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
    spi_txbuff[1] = this->auto_matching_freq;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_SETUP;
    spi_txbuff[1] = 0x07;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::CONFIG;
    spi_txbuff[1] = 0x0F;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_drv::si24r1_cmd::FLUSH_RX;
    spi_txbuff[1] = 0x00; 
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_slave_drv::si24r1_cmd::W_ACK_PAYLOAD;
    spi_txbuff[1] = 0xf0;
    spi_txbuff[2] = this->auto_matching_freq;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,3);
    this->CS.high(); 
    this->auto_matching_flag = true;
    this->CE.high();
    for (uint16_t i = 0; i < 6000; i++)
    {
        #if SI24R1_FREERTOS//freertos
            vTaskDelay(10);
        #else
            delay_ms(10);
        #endif

        if(this->auto_matching_flag == false)
        {
            return true;
        }
    }
    this->CE.low();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
    spi_txbuff[1] = this->channel;
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
    this->CS.high();
    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
    memmove(spi_txbuff+1,this->IP,this->RX_ADR_WIDTH);
    this->CS.low();
    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
    this->CS.high();

    this->auto_matching_flag = false;
    this->CE.high();
    return false;
    
}



