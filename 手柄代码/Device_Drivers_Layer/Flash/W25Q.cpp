#include "W25Q.h"


void W25Q::W25Q_init(void)
{
    this->W25_SPI.spi__init(  SPI0,
                                spi_hal::spi_gpio_group::group0,
                                SPI_MASTER,
                                SPI_TRANSMODE_FULLDUPLEX,
                                SPI_FRAMESIZE_8BIT,
                                SPI_NSS_SOFT,
                                SPI_ENDIAN_MSB,
                                SPI_CK_PL_LOW_PH_1EDGE,
                                SPI_PSC_4);
     gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_2|GPIO_PIN_3);
     this->W25_SPI.spi_switch(ENABLE);
     this->W25_CS.gpio__init(GPIOA,GPIO_PIN_4);
     this->W25_CS.high();
}


uint8_t W25Q::W25QXX_ReadSR(void)
{
    uint8_t byte = 0;
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::ReadStatusReg1);
    byte = this->W25_SPI.readwrite_byte(0xff);
    this->W25_CS.high();
    return byte;
}

void W25Q::W25QXX_Write_SR(uint8_t SR)
{
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(WriteStatusReg1);
    this->W25_SPI.readwrite_byte(SR);
    this->W25_CS.high(); 
}

void W25Q::W25QXX_Write_Enable(void)
{
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::WriteEnable);
    this->W25_CS.high(); 
}

void W25Q::W25QXX_Write_Disable(void)
{
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::WriteDisable);
    this->W25_CS.high();  
}


uint16_t W25Q::W25QXX_ReadID(void)
{
    uint16_t temp = 0;
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(0x90);
    this->W25_SPI.readwrite_byte(0x00);
    this->W25_SPI.readwrite_byte(0x00);
    this->W25_SPI.readwrite_byte(0x00);
    temp|=this->W25_SPI.readwrite_byte(0xff)<<8;
    temp|=this->W25_SPI.readwrite_byte(0xff);
    this->W25_CS.high();
    return temp;
}

void W25Q::W25QXX_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
    uint16_t i;
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::ReadData);
    this->W25_SPI.readwrite_byte((uint8_t)((ReadAddr&0xff0000)>>16));
    this->W25_SPI.readwrite_byte((uint8_t)((ReadAddr&0xff00)>>8));
    this->W25_SPI.readwrite_byte((uint8_t)(ReadAddr&0xff));
    for(i=0;i<=NumByteToRead;i++)
    {
        pBuffer[i] = this->W25_SPI.readwrite_byte(0xFF);
    }
    this->W25_CS.high();
}

void W25Q::W25QXX_Write_Page(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    this->W25QXX_Write_Enable();
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::PageProgram);
    this->W25_SPI.readwrite_byte((uint8_t)((WriteAddr&0xff0000)>>16));
    this->W25_SPI.readwrite_byte((uint8_t)((WriteAddr&0xff00)>>8));
    this->W25_SPI.readwrite_byte((uint8_t)(WriteAddr&0xff));

    for(uint16_t i=0;i<NumByteToWrite;i++)
    {
        this->W25_SPI.readwrite_byte(pBuffer[i]);
    }
    this->W25_CS.high();
    this->W25QXX_Wait_Busy();
}

void W25Q::W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    uint8_t data_read[4096];
    uint16_t i;
    pageremain = 256-WriteAddr%256;
    if(NumByteToWrite<=pageremain)
        pageremain = NumByteToWrite;
    while(1)
    {
        this->W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)
            break;
        else
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;
            NumByteToWrite-=pageremain;
            if(NumByteToWrite>256)
                pageremain = 256;
            else
                pageremain = NumByteToWrite;
        }
    }
        
}

uint8_t W25QXX_BUFFER[4096];
void W25Q::W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *W25QXX_BUF;
    W25QXX_BUF = W25QXX_BUFFER;
    secpos = WriteAddr/4096;
    secoff = WriteAddr%4096;
    secremain = 4096-secoff;
    if(NumByteToWrite<=secremain)
        secremain=NumByteToWrite;
    while (1)
    {
        this->W25QXX_Read(W25QXX_BUF,secpos*4096,4096);
        for(i=0;i<secremain;i++)
        {
            if(W25QXX_BUF[secoff+i]!=0xFF)
            break;
        }
        if(i<secremain)
        {
            this->W25QXX_Erase_Sector(secpos);
            for(i=0;i<secremain;i++)
            {
                W25QXX_BUF[i+secoff] = pBuffer[i];
            }
            this->W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);
        }else
        {
            this->W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);
        }
        if(NumByteToWrite == secremain)
            break;
        else
        {
            secpos++;
            secoff = 0;
            pBuffer+=secremain;
            WriteAddr+=secremain;
            NumByteToWrite-=secremain;
            if(NumByteToWrite>4096)
                secremain = 4096;
            else
                secremain=NumByteToWrite;
        }
    }
    
}

void W25Q::W25QXX_Write_Sector(uint8_t *pBuffer,uint32_t WriteAddr)
{
    //this->W25QXX_Erase_Sector(WriteAddr);
    for (uint16_t i = 0; i < 16; i++)
    {
        this->W25QXX_Write_Page(pBuffer+i*256,WriteAddr+i*256,256);
    }
    
}

void W25Q::W25QXX_Erase_Chip(void)
{
    this->W25QXX_Write_Enable();
    this->W25QXX_Wait_Busy();
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::ChipErase);
    this->W25_CS.high();
    this->W25QXX_Wait_Busy();
}
void W25Q::W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr*=4096;
    this->W25QXX_Write_Enable();
    this->W25QXX_Wait_Busy();
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::SectorErase);
    this->W25_SPI.readwrite_byte((uint8_t)((Dst_Addr)>>16));
    this->W25_SPI.readwrite_byte((uint8_t)((Dst_Addr)>>8));
    this->W25_SPI.readwrite_byte((uint8_t)(Dst_Addr));
    this->W25_CS.high();
    this->W25QXX_Wait_Busy();
}

void W25Q::W25QXX_Wait_Busy(void)
{
    while((this->W25QXX_ReadSR()&0x01)==0x01);
}
void W25Q::W25QXX_PowerDown(void)
{
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::PowerDown);
    this->W25_CS.high();
}
void W25Q::W25QXX_WAKEUP(void)
{
    this->W25_CS.low();
    this->W25_SPI.readwrite_byte(cmd::ReleasePowerDown);
    this->W25_CS.high();
}