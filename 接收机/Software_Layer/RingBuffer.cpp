#include "RingBuffer.h"

void RingBuffer::init(uint8_t *buffer,uint16_t buffer_len)
{
    this->ringBuf=buffer;
    this->Max_len = buffer_len;
    this->head = 0;
    this->tail = 0;
}

bool RingBuffer::write(uint8_t *data)
{
    this->ringBuf[this->tail]=*data;
    if(++this->tail>=this->Max_len)
        this->tail = 0;
    if(this->tail == this->head)
        if(++this->head >= this->Max_len)
            this->head = 0;
    return true;
}

bool RingBuffer::read(uint8_t *data)
{
    if(this->head == this->tail)
        return false;
    else
    {
        *data = this->ringBuf[this->head];
        if(++this->head >= this->Max_len)
            this->head = 0;
        return true;
    }
}

uint16_t RingBuffer::write_packet(uint8_t *data,uint16_t Len)
{
    uint16_t i;
    for(i=0;i<Len;i++)
    {
        this->ringBuf[this->tail]=*(data+i);
        if(++this->tail>=this->Max_len)
            this->tail = 0;
        if(this->tail == this->head)
            if(++this->head >= this->Max_len)
                this->head = 0;
    }
    return i;
}


uint16_t RingBuffer::read_packet(uint8_t *data,uint16_t Len)
{
    uint16_t i;
    

    for(i=0;i<Len;i++)
    {
        if(this->head == this->tail)
            return i;
        *(data+i) = this->ringBuf[this->head];
        if(++this->head >= this->Max_len)
            this->head = 0;
    }
    return Len;

}

