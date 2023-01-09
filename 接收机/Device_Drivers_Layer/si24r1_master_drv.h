#ifndef _si24r1_master_drv_H
#define _si24r1_master_drv_H

#include "si24r1_drv.h"

class si24r1_master_drv:public si24r1_drv
{
private:
    uint8_t Send_buffer[32],Send_Len=0;//发送缓冲区
public:
    si24r1_master_drv()=default;
    uint8_t check_devicd(void);//检查设备
    void si24r1_master_init(void);
    
    void si24r1_Txpacket(uint8_t *txbuf,uint8_t txlen);
    void Send_data(void);

    gpio_hal test;

    uint8_t Master_Recive_buffer[32],Recive_Len=0;
    void (*revice_callback)(uint8_t*,uint8_t)=0;
};









#endif
