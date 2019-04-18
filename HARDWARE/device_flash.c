/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_flash.c
*作用       : 内部flash模拟eeprom
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "system.h"

//EEPROM存储分布
/****************************************************************************************
 *   DevID_H DevID_L| DevSN_H DevSN_L|  PD_H   PD_M   PD_L  |终端 类型 | 通讯方式  |Deveui_H Deveui_L| SoftWareVersion|
 *     0x0800C000   |   0x0800C002   |       0x0800C004     |0x0800C007|0x0800C008 |    0x0800C009   |    0x0800C00B  |
 *
 *
 */


static FLASH_EraseInitTypeDef EraseInitStruct;



/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
   else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_5))*/
  {
    sector = FLASH_SECTOR_5;
  }

  return sector;
}





/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}



void FlashRsvWrite(uint8_t *data_ptr,uint8_t index,uint8_t data_size)
{
    static uint8_t m;
    uint8_t cacheBuf[eepromsize];
    uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
    uint32_t SectorError = 0;
    uint32_t dataToWrite;
    
    for(m=0;m<eepromsize;m++)
      cacheBuf[m] = FlashRead(m);
    
    for(m=0;m<data_size;m++)
      cacheBuf[index++] = *data_ptr++;
    
    EnterCritical();
    
    HAL_FLASH_Unlock();
    
//    FirstSector = FLASH_SECTOR_0;
//    NbOfSectors = 1;
    
    FirstSector = GetSector(addrHead);
    NbOfSectors = GetSector(addrHead) - FirstSector + 1;

    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = FirstSector;
    EraseInitStruct.NbSectors    = NbOfSectors;
    
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    { 
      Error_Handler();
    }
    
    __HAL_FLASH_DATA_CACHE_DISABLE();
    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

    __HAL_FLASH_DATA_CACHE_RESET();
    __HAL_FLASH_INSTRUCTION_CACHE_RESET();

    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    
    Address = addrHead;

    for(m=0;m<eepromsize/4;m++)
    {
//        dataToWrite = ((uint32_t)cacheBuf[4*m])<<24 + ((uint32_t)cacheBuf[4*m + 1])<<16 +
//          ((uint32_t)cacheBuf[4*m + 2])<<8 + cacheBuf[4*m + 3];
        
        Byte0(dataToWrite) = cacheBuf[4*m];
        Byte1(dataToWrite) = cacheBuf[4*m+1];
        Byte2(dataToWrite) = cacheBuf[4*m+2];
        Byte3(dataToWrite) = cacheBuf[4*m+3];
        
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, dataToWrite) == HAL_OK)
        {
          Address = Address + 4;
        }
        else
        { 
          /*
            FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
          */
          Error_Handler();
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock(); 
    Address = 0;
    
    ExitCritical();
}


uint8_t FlashRead(uint8_t index)
{
    uint32_t *Flash_ptrD;
    Flash_ptrD = (uint32_t *)addrHead;
    uint8_t *flash_ptr =  ((uint8_t *)Flash_ptrD) + index;

    return *flash_ptr;
}


void DevInforProgram(uint8_t *data_ptr,uint8_t index,uint8_t data_size)
{
    static uint8_t cache[7];
    
    switch(index)
    {
        case 0x00:
          cache[0] = data_ptr[5];
          cache[1] = data_ptr[6];
          cache[2] = data_ptr[7];
          cache[3] = data_ptr[8];
          cache[4] = data_ptr[2];
          cache[5] = data_ptr[3];
          cache[6] = data_ptr[4];
          break;
        case 0x07:
          cache[0] = data_ptr[2];
          cache[1] = data_ptr[3];
          cache[2] = data_ptr[4];
          cache[3] = data_ptr[5];
          cache[4] = data_ptr[6];
          cache[5] = data_ptr[7];
          cache[6] = data_ptr[8];
          break;
    }
    
    FlashRsvWrite(cache,index,data_size);
}



