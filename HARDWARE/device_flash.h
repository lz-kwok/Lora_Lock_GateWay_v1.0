/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_flash.h
*作用       : 内部flash模拟eeprom
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_FLASH__H
#define __DEVICE_FLASH__H

#include <stdint.h>

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */


#define addrHead                ADDR_FLASH_SECTOR_5
#define eepromsize              128





uint32_t GetSector(uint32_t Address);
uint32_t GetSectorSize(uint32_t Sector);
void FlashRsvWrite(uint8_t *data_ptr,uint8_t index,uint8_t data_size);
uint8_t FlashRead(uint8_t index);
void DevInforProgram(uint8_t *data_ptr,uint8_t index,uint8_t data_size);



#endif /* __DEVICE_FLASH__H */
