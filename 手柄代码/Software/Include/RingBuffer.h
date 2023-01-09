#ifndef _RingBuffer_H
#define _RingBuffer_H

#include "gd32f30x_libopt.h"

class RingBuffer
{
private:
    uint16_t Max_len,head,tail;
    uint8_t *ringBuf;
public:
    RingBuffer()=default;
    void init(uint8_t *buffer,uint16_t buffer_len);
    bool write(uint8_t *data);
    bool read(uint8_t *data);
    uint16_t write_packet(uint8_t *data,uint16_t Len);
    uint16_t read_packet(uint8_t *data,uint16_t Len);
};




#endif