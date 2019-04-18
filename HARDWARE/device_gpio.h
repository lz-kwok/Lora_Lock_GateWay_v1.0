/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_gpio.h
*作用       : IO控制
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/
#ifndef __DEVICE_GPIO_H
#define __DEVICE_GPIO_H

#include "system.h"



#define  EnablePS_StaGPIO() __GPIOA_CLK_ENABLE()
#define  StaPORT            GPIOA 
#define  StaPin             GPIO_PIN_10
#define  CtrlPin            GPIO_PIN_5  

#define  LEDOn()            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define  LEDOff()           HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)

#define  OledPowerOn()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET)
#define  OledPowerOff()     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET)
#define  DTUPowerOn()       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET)
#define  DTUPowerOff()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET)
#define  CtrPowerOn()       HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET)
#define  CtrPowerOff()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET)

   
#define  LCD_RST_CLR()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET)
#define  LCD_RST_SET()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET)

#define  LCD_DC_CLR()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET)
#define  LCD_DC_SET()	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET)
   
#define  SETA_CLR()         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET)
#define  SETA_SET()         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET)
#define  SETB_CLR()         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET)
#define  SETB_SET()         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET)

void Gpio_init(void);




#endif
