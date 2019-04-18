/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_key.h
*作用       : 按键设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_KEY_H
#define __DEVICE_KEY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "system.h"
   
extern struct rt_timer soft_timer3;
   
#define Key1Press      0x10
#define Key1Release    0x11
#define Key2Press      0x20
#define Key2Release    0x21
#define Key3Press      0x30
#define Key3Release    0x31
#define Key4Press      0x40
#define Key4Release    0x41
#define Key5Press      0x50
#define Key5Release    0x51
#define Key6Press      0x60
#define Key6Release    0x61
#define Key7Press      0x70
#define Key7Release    0x71
#define Key8Press      0x80
#define Key8Release    0x81
#define Key9Press      0x90
#define Key9Release    0x91
#define Key11Press     0xB0
#define Key11Release   0xB1
#define Key12Press     0xC0
#define Key12Release   0xC1
#define Key13Press     0xD0
#define Key13Release   0xD1
#define Key14Press     0xE0
#define Key14Release   0xE1
#define MultiKey1      0xF9
#define MultiKey2      0xF8


typedef enum 
{  
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;



/** @defgroup STM32F4XX_LOW_LEVEL_BUTTON STM32F4XX LOW LEVEL BUTTON
  * @{
  */  
#define BUTTONn                                 1  

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         GPIO_PIN_3
#define USER_BUTTON_GPIO_PORT                   GPIOB
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()   
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()  
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_3
#define USER_BUTTON_EXTI_IRQn                   EXTI3_IRQn
    
#define FunctionPin1                            GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_13                   
#define FunctionPort1                           GPIOB
#define FunctionPin2                            GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12                  
#define FunctionPort2                           GPIOC  
#define FunctionPort2Enable                     __HAL_RCC_GPIOC_CLK_ENABLE()  
#define ChargingPin                             GPIO_PIN_1
#define FunctionPin3                            GPIO_PIN_15                
#define FunctionPort3                           GPIOA     
#define FunctionPort3Enable                     __HAL_RCC_GPIOA_CLK_ENABLE()  

    
#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)      USER_BUTTON_GPIO_CLK_ENABLE()
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)     USER_BUTTON_GPIO_CLK_DISABLE()

/* Aliases */
#define KEY_BUTTON_PIN                          USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                    USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()            USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()           USER_BUTTON_GPIO_CLK_DISABLE()
#define KEY_BUTTON_EXTI_LINE                    USER_BUTTON_EXTI_LINE
#define KEY_BUTTON_EXTI_IRQn                    USER_BUTTON_EXTI_IRQn
    
#define KEYBOARD_PIN_LINE                       GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
#define KEYBOARD_PIN_ROW                        GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define KEYBOARD_PORT                           GPIOB
#define KEYBOARD_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()   
#define KEYBOARD_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()  


#define KEY_DATA_IN()  	GPIOB->BSRR=0x88888888
#define KEY_DATA_INPUT 	GPIOB->IDR
#define KEY_DATA_OUT() 	GPIOB->BSRR=0x33333333
#define KEY_DATA_OUTPUT GPIOB->ODR

   
  
/* Exported functions ------------------------------------------------------- */
void Key_init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t GetKeyState(Button_TypeDef Button);
//void KeyScan1000Routine(void);
void KeyProcess(uint8_t data);
void ShowString(uint8_t *data,uint8_t *sh,uint8_t len);


/**
  * @}
  */ 
void thread_Key_scan_entry(void* parameter);


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_KEY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
