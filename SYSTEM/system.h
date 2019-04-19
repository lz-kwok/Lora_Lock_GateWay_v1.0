#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "rtthread.h"

#include "stm32f4xx.h"

//////////////////////////////////////////////////////////////////////////////////	 

//All rights reserved by Glz
//********************************************************************************
//修改说明
//无
//////////////////////////////////////////////////////////////////////////////////  

//////////////////////////////////////////////////////////////////////////////////	 

//Macro Definition of Function Modules by Glz
//********************************************************************************
//功能模块宏定义
//无
//////////////////////////////////////////////////////////////////////////////////  
//#define USB_VCP
//#define BME280_MODUOLE
//#define SOC_RTC
#define TurnOnPrintf

#ifdef TurnOnPrintf
#define Printf2Uart1
#endif


//0,不支持os
//1,支持os
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持OS
///////////////////////////////////////////////////////////////////////////////////

#define HSE               (0x01)
#define HSI               (0x02)
#define HSE_OK            (0x03)
#define HSE_error         (0x04)
#define HSI_OK            (0x05)
#define HSI_error         (0x06)
#define Parameter_error   (0x07)

#define HSE_CRYSTAL


//定义一些常用的数据类型短关键字 
typedef int32_t  s32;
typedef int16_t	 s16;
typedef int8_t 	 s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t 	sc8;  

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t	 	vsc32;  
typedef __I int16_t 	vsc16; 
typedef __I int8_t 		vsc8;   

typedef uint32_t  u32;
typedef uint16_t 	u16;
typedef uint8_t  	u8;

typedef const uint32_t 	uc32;  
typedef const uint16_t 	uc16;  
typedef const uint8_t 	uc8; 

typedef __IO uint32_t  	vu32;
typedef __IO uint16_t 	vu16;
typedef __IO uint8_t  	vu8;

typedef __I uint32_t 	vuc32;  
typedef __I uint16_t 	vuc16; 
typedef __I uint8_t 	vuc8;  


	 
//位带操作,实现51类似的GPIO控制功能
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014 
#define GPIOJ_ODR_ADDr    (GPIOJ_BASE+20) //0x40022414
#define GPIOK_ODR_ADDr    (GPIOK_BASE+20) //0x40022814

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16) //0x40022410 
#define GPIOK_IDR_Addr    (GPIOK_BASE+16) //0x40022810 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入

#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  //输出 
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  //输入

#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  //输出 
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  //输入

#include "data.h"
#include "systemglobal.h"
#include "device_usart.h"
#include "device_gpio.h"
#include "device_key.h"
#include "device_iic.h"
#include "device_spi.h"
#include "device_adc.h"
#include "device_flash.h"
#include "device_menu.h"
#include "device_oled.h"
#include "device_433.h"
#include "device_crc.h"  
#include "device_atk_as608.h"
#include "device_printf.h"


#ifdef SOC_RTC
#include "device_rtc.h"
#endif

#ifdef USB_VCP
#include "device_usb.h"
#include "usbd_cdc_if.h"
#endif

#ifdef BME280_MODUOLE
#include "bme280.h"
#endif

#define	EnterCritical()		        __disable_irq(); 
#define ExitCritical()		        __enable_irq();

#define SetBit(data, offset)        ((data) |= 1U << (offset))      // 置位
#define ResetBit(data, offset)      ((data) &= ~(1U << (offset)))   // 复位
#define GetBit(data, offset)        (((data) >> (offset)) & 0x01)   // 获取位

#define Byte0(data)                 ((uint8_t *)(&(data)))[0]
#define Byte1(data)                 ((uint8_t *)(&(data)))[1]
#define Byte2(data)                 ((uint8_t *)(&(data)))[2]
#define Byte3(data)                 ((uint8_t *)(&(data)))[3]

extern DataStruct *AppDataPointer;
extern AppStruct  App;

u8 HexToAscii(u8 hex);
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void HSE_Clock_Init(uint8_t MainFreq);//时钟系统配置
void HSI_Clock_Init(uint8_t MainFreq);
void Error_Handler( void );


typedef struct
{   
    struct Device
    {     
        struct Adc
        {
            void (*Register)(AdcChannelEnum channel, uint16_t * dataPointer);
        }Adc;
        
        struct Usart1
        {
            void (*WriteData)(uint8_t data);
            void (*WriteNData)(uint8_t *data,uint16_t len);
            void (*WriteDMAData)(uint8_t *data,uint16_t len);
            void (*WriteString)(uint8_t  *s);
        }Usart1;	

        struct Usart2
        {
            void (*WriteData)(uint8_t data);
            void (*WriteString)(uint8_t  *s);
        }Usart2;
        
        struct Usart6
        {
            void (*WriteData)(uint8_t data);
            void (*WriteString)(uint8_t  *s);
        }Usart6;

        struct Iic1
        {
            uint8_t (*WriteData)(uint8_t SlaveAdd,uint8_t reg,uint8_t *data,uint16_t num);
            uint8_t (*ReadData)(uint8_t SlaveAdd,uint8_t ReadAdd,uint8_t *data,uint16_t NumToRead);
        }Iic1;	
     
        struct Spi1
        {
            uint8_t (*WriteReadData)(uint8_t TxData);
        }Spi1;	
		
//        struct Timer
//        {
//            void (*Start)(int id, TimerModeEnum mode, int times, function functionPointer);
//            void (*Stop)(int id); 
//        }Timer;
        
        struct Pwm
        {
            void (*SetDutyCycle)(uint32_t DutyDat);
        }Pwm;
        
    }Device;
}SystemStruct;

extern SystemStruct System;


//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 
#endif

