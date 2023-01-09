#ifndef _W25Q_H
#define _W25Q_H


#include "spi_hal.h"
#include "gpio_hal.h"

class W25Q
{
private:
    spi_hal W25_SPI;
    gpio_hal W25_CS;
public:

    enum cmd
    {
        WriteEnable      = 0x06,
        WriteDisable     = 0x04,
        ReadStatusReg1   = 0x05,
        WriteStatusReg1  = 0x01,
        ReadStatusReg2   = 0x35,
        WriteStatusReg2  = 0x31,
        ReadData		 = 0x03,
        FastReadData	 = 0x0B,
        FastReadDual	 = 0x3B,
        PageProgram		 = 0x02,
        BlockErase		 = 0xD8,
        SectorErase		 = 0x20,
        ChipErase		 = 0xC7,
        PowerDown		 = 0xB9,
        ReleasePowerDown = 0xAB,
        DeviceID	     = 0xAB, 
        ManufactDeviceID = 0x90,
        JedecDeviceID    = 0x9F,
        Nop              = 0xff
    };

    W25Q()=default;
    void W25Q_init(void);
    uint8_t W25QXX_ReadSR(void);
    void W25QXX_Write_SR(uint8_t SR);
    void W25QXX_Write_Enable(void);
    void W25QXX_Write_Disable(void);
    uint16_t W25QXX_ReadID(void);
    void W25QXX_Read(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
    void W25QXX_Write_Page(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
    void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
    void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);  
    void W25QXX_Write_Sector(uint8_t *pBuffer,uint32_t WriteAddr);
    void W25QXX_Erase_Chip(void);
    void W25QXX_Erase_Sector(uint32_t Dst_Addr);
    void W25QXX_Wait_Busy(void); 
    void W25QXX_PowerDown(void);
    void W25QXX_WAKEUP(void);
};















#endif
