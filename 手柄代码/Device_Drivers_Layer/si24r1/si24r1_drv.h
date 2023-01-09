#ifndef _si24r1_drv_H
#define _si24r1_drv_H
#include "spi_hal.h"
#include "gpio_hal.h"
#include "exti_hal.h"
#include "string.h"
#include "System_time.h"
#include "timer_hal.h"
#include "RingBuffer.h"


class si24r1_drv
{
    private:


    public:
        si24r1_drv()=default;
        enum si24r1_register
        {
            CONFIG          = 0x00,
            EN_AA           = 0x01,
            EN_RXADDR       = 0x02,
            SETUP_AW        = 0x03,
            SETUP_RETR      = 0x04,
            RF_CH           = 0x05,
            RF_SETUP        = 0x06,
            STATUS          = 0x07,
            OBSERVE_TX      = 0x08,
            RSSI            = 0x09,
            RX_ADDR_P0      = 0x0A,
            RX_ADDR_P1      = 0x0B,
            RX_ADDR_P2      = 0x0C,
            RX_ADDR_P3      = 0x0D,
            RX_ADDR_P4      = 0x0E,
            RX_ADDR_P5      = 0x0F,
            TX_ADDR         = 0x10,
            RX_PW_P0        = 0x11,
            RX_PW_P1        = 0x12,
            RX_PW_P2        = 0x13,
            RX_PW_P3        = 0x14,
            RX_PW_P4        = 0x15,
            RX_PW_P5        = 0x16,
            NRF_FIFO_STATUS = 0x17,
            DYNPD           = 0x1C,
            FEATURE         = 0x1D            
        };
        enum irq_state
        {
            MAX_TX      =0x10,//最大重发次数中断
            TX_OK       =0x20,//发送完成中断
            RX_OK       =0x40,//接收到数据中断
            TRX_OK      =0x60,//发送完成接收到数据同时产生
            RXFIFO_EMPTY=0x0e //RX FIFO为空
        };
        enum si24r1_cmd
        {
            NRF_READ_REG = 0x00,
            NRF_WRITE_REG= 0x20,
            RD_RX_PLOAD  = 0x61,
            WR_TX_PLOAD  = 0xA0,
            FLUSH_TX     = 0xE1,
            FLUSH_RX     = 0xE2,
            REUSE_TX_PL  = 0xE3,
            R_RX_PL_WID  = 0x60,
            W_ACK_PAYLOAD= 0xA8,
            NOP          = 0xFF
        };

        enum si24r1_state
        {
            data_send,
            freq_toggle,
            auto_matching
        };
        enum si24r1_send_states
        {
            send_succes,
            sending,
            send_err
        };

        spi_hal si24r1_spi;
        timer_hal si24r1_timer;
        gpio_hal CS,CE,text;
        exti_hal irq;
        RingBuffer recive_buffer,send_buffer;
        uint8_t RX_ADR_WIDTH=5;
        uint8_t TX_ADR_WIDTH=5;
        uint8_t channel=90;
        uint8_t IP[5] = {'2','5','3','d','s'};

        uint8_t recive_data[512]={0};//接收缓冲区
        uint8_t send_data[512]={0};//发送缓冲区


        void init(void);
};




#endif
