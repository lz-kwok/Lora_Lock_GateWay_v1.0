/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_gpio.c
*作用       : IO控制
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/
#include "device_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 


//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void Gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    EnablePS_StaGPIO();
    
    /* Configure the GPIO_INPUT pin */
    GPIO_InitStruct.Pin   = StaPin;           //初始化PA10为下拉输入	
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  //读摸出感应状态(触摸感应时输出高电平信号)
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    
    HAL_GPIO_Init(StaPORT, &GPIO_InitStruct);
    
    /* Configure the GPIO_CTRL pin */
    GPIO_InitStruct.Pin   = CtrlPin;           
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    
    HAL_GPIO_Init(StaPORT, &GPIO_InitStruct);
    
    LEDOff();
}
