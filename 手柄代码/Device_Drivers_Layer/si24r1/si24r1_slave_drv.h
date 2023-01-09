#ifndef _si24r1_slave_drv_H
#define _si24r1_slave_drv_H

#include "si24r1_drv.h"

class si24r1_slave_drv:public si24r1_drv
{
private:
    /* data */
public:
    uint8_t Recive_Len=0;
    si24r1_slave_drv()=default;
    uint8_t check_devicd(void);
    void si24r1_slave_init(void);
    void (*revice_callback)(uint8_t*,uint8_t)=0;

    //切频部分
    uint8_t toggle_channel;
    bool si24r1_toggle_state = false;
    
    //自动对频部分
    uint8_t auto_matching_freq;
    bool si24r1_auto_matching(void);
    uint8_t auto_matching_IP[5]={'u','n','d','e','r'};
};




#endif
