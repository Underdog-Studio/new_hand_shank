#include "si24r1_master_drv.h"

void si24r1_master_irq_callback(void *str);
void si24r1_master_timer_callback(void *str);


void si24r1_master_drv::si24r1_master_init(void)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6];
    this->init();
    this->irq.init( EXTI_10,
                    GPIOC,
                    4,
                    0,
                    EXTI_INTERRUPT,
                    EXTI_TRIG_FALLING,
                    GPIO_PUPD_PULLUP);
    this->irq.Callback_Register(this,si24r1_master_irq_callback);

    
    this->si24r1_timer.timer__init( TIMER4,
                                    400-1,
                                    2250*2-1,//2250
                                    5,
                                    TIMER_COUNTER_EDGE,
                                    TIMER_COUNTER_DOWN);
    this->si24r1_timer.Callback_Register(this,si24r1_master_timer_callback);

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
    spi_txbuff[1] = 0x07;
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
    if(this->si24r1_master_send_state == si24r1_drv::si24r1_send_states::send_succes
        || this->si24r1_master_send_state == si24r1_drv::si24r1_send_states::send_err)
    {
        this->CE.low();
        spi_txbuff[0] = si24r1_cmd::FLUSH_TX;
        spi_txbuff[1] = 0x00;
        this->CS.low();
        this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        this->CS.high();
        spi_txbuff[0] = 0x00;
        this->Send_Len = this->send_buffer.read_packet(spi_txbuff+1,31);
        this->CS.low();
        this->si24r1_spi.readwrite_byte(si24r1_cmd::WR_TX_PLOAD);
        this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->Send_Len+1);
        this->CS.high();
        this->CE.high();
        this->si24r1_master_send_state = si24r1_drv::si24r1_send_states::sending;
    }
}

bool si24r1_master_drv::si24r1_toggle_channel(uint8_t _channel)
{
    uint8_t count=0;
    this->toggle_channel = _channel;
    this->si24r1_master_state = si24r1_drv::si24r1_state::freq_toggle;
    while(this->si24r1_master_state==si24r1_drv::si24r1_state::freq_toggle)
    {
        #if SI24R1_FREERTOS//freertos
            vTaskDelay(10);
        #else
            delay_ms(10);
        #endif
        if(count++>20)
        {
            return false;
        }
    }
    if(this->toggle_channel == this->channel)
        return true;
    else
        return false;
}

bool si24r1_master_drv::si24r1_auto_matching(void)
{
    if(this->si24r1_master_state != si24r1_drv::si24r1_state::auto_matching
        && auto_matching_flag == false)
    {
        this->si24r1_master_state = si24r1_drv::si24r1_state::auto_matching;
        auto_matching_flag = true;
        return false;
    }
    if(this->si24r1_master_state == si24r1_drv::si24r1_state::data_send)
    {
        auto_matching_flag = false;
        return true;
    }
    return false;
} 

void si24r1_master_drv::Send_cmd(uint8_t cmd,uint8_t *data)
{
    uint8_t spi_txbuff[10],spi_rxbuff[10];
    if(this->si24r1_master_send_state == si24r1_drv::si24r1_send_states::send_succes
        || this->si24r1_master_send_state == si24r1_drv::si24r1_send_states::send_err)
    {
        this->CE.low();

        spi_txbuff[0] = si24r1_cmd::FLUSH_TX;
        spi_txbuff[1] = 0x00;
        this->CS.low();
        this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
        this->CS.high();

        switch (cmd)
        {
        case 0x0f:
            spi_txbuff[0] = 0x0f;
            spi_txbuff[1] = *data;
            this->CS.low();
            this->si24r1_spi.readwrite_byte(si24r1_cmd::WR_TX_PLOAD);
            this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
            this->CS.high();
            break;
        case 0xf0:
            spi_txbuff[0] = 0xf0;
            memmove(spi_txbuff+1,data,5);
            this->CS.low();
            this->si24r1_spi.readwrite_byte(si24r1_cmd::WR_TX_PLOAD);
            this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,6);
            this->CS.high();
        break;
        default:
            break;
        }
        this->si24r1_master_send_state = si24r1_drv::si24r1_send_states::sending;
        this->CE.high();
        
    }

}




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
        ((si24r1_master_drv*)str)->si24r1_master_send_state = si24r1_drv::si24r1_send_states::send_err;
        ((si24r1_master_drv*)str)->si24r1_connect_state_count--;
        if(((si24r1_master_drv*)str)->si24r1_connect_state_count<0)
        {
            ((si24r1_master_drv*)str)->si24r1_connect_state_count =0;
            ((si24r1_master_drv*)str)->si24r1_connect_state = false;
        }
    }else if(state & si24r1_drv::irq_state::TX_OK)
    {
        ((si24r1_master_drv*)str)->si24r1_connect_state_count++;
        if(((si24r1_master_drv*)str)->si24r1_connect_state_count>10)
        {
            ((si24r1_master_drv*)str)->si24r1_connect_state_count =10;
            ((si24r1_master_drv*)str)->si24r1_connect_state = true;
        }

        if(state & si24r1_drv::irq_state::RXFIFO_EMPTY)
        {
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
            ((si24r1_master_drv*)str)->si24r1_master_send_state = si24r1_drv::si24r1_send_states::send_succes;
        }else
        {
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
            switch (spi_rxbuff[0])
            {
                case 0x00:
                    (*(si24r1_master_drv*)str).recive_buffer.write_packet(spi_rxbuff+1,(*(si24r1_master_drv*)str).Recive_Len-1);
                    break;
                case 0x0f:
                    ((si24r1_master_drv*)str)->si24r1_slave_response = true;
                    break; 
                case 0xf0:
                    ((si24r1_master_drv*)str)->auto_matching_response = true;
                    ((si24r1_master_drv*)str)->auto_matching_freq = spi_rxbuff[1];
                    break; 
                default:
                    break;
            }
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

            ((si24r1_master_drv*)str)->si24r1_master_send_state = si24r1_drv::si24r1_send_states::send_succes;
        }
    }
}

void si24r1_master_timer_callback(void *str)
{
    if(((si24r1_master_drv*)str)->si24r1_master_state == si24r1_master_drv::si24r1_state::data_send)
        ((si24r1_master_drv*)str)->Send_data(); 
}

void si24r1_master_drv::si24r1_master_tick(void)
{
    uint8_t spi_txbuff[6],spi_rxbuff[6];
    switch (this->si24r1_master_state)
    {
        case si24r1_state::freq_toggle :
           
            this->toggle_count = 0;
            for(;this->toggle_count<this->toggle_retries_number;this->toggle_count++)
            {
                this->Send_cmd(0x0f,&(this->toggle_channel));
                
                #if SI24R1_FREERTOS//freertos
                    vTaskDelay(10);
                #else
                    delay_ms(10);
                #endif
                spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
                spi_txbuff[1] = this->toggle_channel;
                this->CS.low();
                this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                this->CS.high();

                this->Send_cmd(0x0f,&(this->toggle_channel));
                #if SI24R1_FREERTOS//freertos
                    vTaskDelay(30);
                #else
                    delay_ms(30);
                #endif
                if(this->si24r1_master_send_state == si24r1_send_states::send_succes
                    && this->si24r1_slave_response == true)
                {
                    this->channel = this->toggle_channel;
                    break;
                }else
                {
                    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
                    spi_txbuff[1] = this->channel;
                    this->CS.low();
                    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    this->CS.high();
                }
            }
            this->si24r1_master_state = si24r1_state::data_send;
            break;
        case si24r1_state::auto_matching :
            if(this->si24r1_master_send_state != si24r1_send_states::sending)
            {
                spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::TX_ADDR;
                memmove(spi_txbuff+1,this->auto_matching_IP,this->TX_ADR_WIDTH);
                this->CS.low();
                this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->TX_ADR_WIDTH+1);
                this->CS.high();
                spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
                this->CS.low();
                this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
                this->CS.high();
                this->auto_matching_response = false;
                for(uint8_t i=0;i<63;i++)
                {
                    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
                    spi_txbuff[1] = i*2;
                    this->CS.low();
                    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    this->CS.high();
                    #if SI24R1_FREERTOS//freertos
                        vTaskDelay(10);
                    #else
                        delay_ms(10);
                    #endif
                    for(uint8_t j = 0;j<10;j++)
                    {
                        this->Send_cmd(0xf0,this->IP);
                        while (this->si24r1_master_send_state == si24r1_send_states::sending)
                        {
                            vTaskDelay(10);
                        }
                        if(this->auto_matching_response == true)
                        {
                            this->channel = this->auto_matching_freq;
                            spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
                            spi_txbuff[1] = this->channel;
                            this->CS.low();
                            this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                            this->CS.high();
                            spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::TX_ADDR;
                            memmove(spi_txbuff+1,this->IP,this->TX_ADR_WIDTH);
                            this->CS.low();
                            this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->TX_ADR_WIDTH+1);
                            this->CS.high();
                            spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RX_ADDR_P0;
                            this->CS.low();
                            this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,this->RX_ADR_WIDTH+1);
                            this->CS.high();

                            this->si24r1_master_state = si24r1_state::data_send;
                            break;
                        }
                    }  
                    if(this->auto_matching_response == true)
                    {
                        this->auto_matching_response = false;
                        break;
                    }
                }
                if(this->si24r1_master_state != si24r1_state::data_send)
                {
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
                    spi_txbuff[0] = si24r1_cmd::NRF_WRITE_REG + si24r1_register::RF_CH;
                    spi_txbuff[1] = this->channel;
                    this->CS.low();
                    this->si24r1_spi.readwrite_packet(spi_txbuff,spi_rxbuff,2);
                    this->CS.high();
                    this->si24r1_master_state = si24r1_state::data_send;
                }
            }
            break;
        default:
        break;
    }
}
