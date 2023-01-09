#ifndef _si24r1_master_drv_H
#define _si24r1_master_drv_H

#include "si24r1_drv.h"

#define SI24R1_FREERTOS 1 //是否使用FREERTOS

#if SI24R1_FREERTOS
    #include "FreeRTOS.h"
    #include "task.h"
#endif 
class si24r1_master_drv:public si24r1_drv
{
private:
    uint8_t Send_buffer[32];//发送缓冲区
public:
    si24r1_master_drv()=default;

    void si24r1_master_init(void);
    uint8_t check_devicd(void);
    int8_t si24r1_connect_state_count = 0;
    bool si24r1_connect_state = false;
    si24r1_send_states si24r1_master_send_state = si24r1_send_states::send_succes;
    si24r1_state si24r1_master_state = si24r1_state::data_send;

    //频道切换部分
    uint8_t toggle_channel;
    uint8_t toggle_count;
    bool si24r1_slave_response = false;
    uint8_t toggle_retries_number = 3;
    bool si24r1_toggle_channel(uint8_t _channel = 40);

    //自动对频部分
    bool auto_matching_flag=false;
    bool auto_matching_response = false;
    uint8_t auto_matching_freq; 
    uint8_t auto_matching_IP[5]={'u','n','d','e','r'};
    bool si24r1_auto_matching(void);


    //数据/命令传输部分
    uint8_t Master_Recive_buffer[32],Recive_Len=0,Send_Len = 0;
    void si24r1_Txpacket(uint8_t *txbuf,uint8_t txlen);
    void Send_data(void);
    void Send_cmd(uint8_t cmd,uint8_t *data);
    void (*revice_callback)(uint8_t*,uint8_t)=0;



    void si24r1_master_tick(void);
};









#endif
