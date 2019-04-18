/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_iic.h
*作用       : iic设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_IIC__H
#define __DEVICE_IIC__H

#include "system.h"

/* Includes ------------------------------------------------------------------*/
#define IIC_SPEED                            50000                 
#define IIC_TIMEOUT_MAX                      0x1000        /*<! The value of the maximal timeout for BUS waiting loops */

/* I2C peripheral configuration defines */
#define IIC_EXPBD                            I2C1
#define IIC_EXPBD_CLK_ENABLE()               __I2C1_CLK_ENABLE()
#define IIC_EXPBD_SCL_SDA_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define IIC_EXPBD_SCL_SDA_AF                 GPIO_AF4_I2C1
#define IIC_EXPBD_SCL_SDA_GPIO_PORT          GPIOB
#define IIC_EXPBD_SCL_PIN                    GPIO_PIN_8
#define IIC_EXPBD_SDA_PIN                    GPIO_PIN_9

#define IIC_EXPBD_FORCE_RESET()              __I2C1_FORCE_RESET()
#define IIC_EXPBD_RELEASE_RESET()            __I2C1_RELEASE_RESET()

/* I2C interrupt requests */
#define IIC_EXPBD_EV_IRQn                    I2C1_EV_IRQn
#define IIC_EXPBD_ER_IRQn                    I2C1_ER_IRQn




uint8_t IIC_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
uint8_t IIC_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size );
uint8_t IIC1_Init(void);



#endif /* __DEVICEUART__H */
