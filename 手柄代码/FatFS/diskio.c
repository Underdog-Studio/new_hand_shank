/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "main.h"
/* Definitions of physical drive number for each drive */
// #define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
// #define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
// #define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_FLASH 0
#define DEV_SPI_FLASH 1
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)读扇区                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	uint32_t read_address;
	int result;
	switch (pdrv)
	{
	case DEV_FLASH:
		read_address = 0x8040000+sector*4096;
		for (uint32_t i = 0; i < count*4096; i+=4)
		{
			*(uint32_t*)(buff+i) = *(uint32_t*)(read_address+i);
		}
		return RES_OK;
	case DEV_SPI_FLASH:
		read_address = sector*4096;
		SPI_FLASH_Read(buff,sector*4096,count*4096);

		return RES_OK;
	default:
		return RES_PARERR;
	}
	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	uint32_t Write_address;
	int result;
	switch (pdrv)
	{
	case DEV_FLASH:
		fmc_bank0_unlock();
		fmc_bank1_unlock();
		while (count--)
		{
			Write_address = 0x08040000+sector*4096;
			if(Write_address<0x08080000)
			{
				fmc_page_erase(Write_address);//前512K--2K大小页
				fmc_page_erase(Write_address+2048);//前512K--2K大小页
			}else
			{
				fmc_page_erase(Write_address);//后512--4K
			}
			for(uint16_t i=0;i<4096;i+=4)
			{
				while(1)
				{
					if (*(uint32_t*)(Write_address+i) != *(uint32_t*)(buff+i))       //�����Ƿ���ͬ
					{
						fmc_word_program(Write_address+i, *(uint32_t*)(buff+i));			//��ͬ ����
					}else
					{
						break;                                                              //����Ҫ����
					}
				}
			}
			sector++;
			buff+=4096;
		}
		fmc_bank0_lock();
		fmc_bank1_lock();
		return RES_OK;
	case DEV_SPI_FLASH:
		while (count--)
		{
			SPI_FLASH_Write(buff,sector*4096,4096);
			sector++;
			buff+=4096;
		}
		return RES_OK;
	default:
		return RES_PARERR;
	}
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;
	switch (pdrv)
	{
		case DEV_FLASH:
			switch (cmd)
			{
				case CTRL_SYNC:
					return RES_OK;
				case GET_SECTOR_COUNT:
					*(uint32_t*)buff = 192;//获取扇区数1024-256/4k
					return RES_OK; 
				case GET_SECTOR_SIZE:
					*(uint16_t*)buff = 4096;//获取扇区大小4k
					return RES_OK;
				case GET_BLOCK_SIZE:
					*(uint32_t*)buff = 192*4096;//获取块大小256k
					return RES_OK;
			}
		case DEV_SPI_FLASH:
			switch (cmd)
			{
				case CTRL_SYNC:
					return RES_OK;
				case GET_SECTOR_COUNT:
					*(uint32_t*)buff = 1024;//获取扇区数1024-256/4k
					return RES_OK; 
				case GET_SECTOR_SIZE:
					*(uint16_t*)buff = 4096;//获取扇区大小4k
					return RES_OK;
				case GET_BLOCK_SIZE:
					*(uint32_t*)buff = 1024*4096;//获取块大小256k
					return RES_OK;
			}
		default:
			return RES_PARERR;
	}
	
}

