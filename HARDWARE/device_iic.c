/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_iic.c
*作用       : iic设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"



static uint32_t I2C_Timeout = IIC_TIMEOUT_MAX;    /*<! Value of Timeout when I2C communication fails */
I2C_HandleTypeDef I2C1_Handle;



/**
 * @brief I2C1 MSP Initialization
 * @param None
 * @retval None
 */
static void IIC1_MspInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable I2C GPIO clocks */
    IIC_EXPBD_SCL_SDA_GPIO_CLK_ENABLE();

    /* I2C_EXPBD SCL and SDA pins configuration -------------------------------------*/
    GPIO_InitStruct.Pin        =   IIC_EXPBD_SCL_PIN | IIC_EXPBD_SDA_PIN;
    GPIO_InitStruct.Mode       =   GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed      =   GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Pull       =   GPIO_NOPULL;
    GPIO_InitStruct.Alternate  =   IIC_EXPBD_SCL_SDA_AF;

    HAL_GPIO_Init(IIC_EXPBD_SCL_SDA_GPIO_PORT, &GPIO_InitStruct );

    /* Enable the I2C_EXPBD peripheral clock */
    IIC_EXPBD_CLK_ENABLE();

    /* Force the I2C peripheral clock reset */
    IIC_EXPBD_FORCE_RESET();

    /* Release the I2C peripheral clock reset */
    IIC_EXPBD_RELEASE_RESET();

//    /* Enable and set I2C_EXPBD Interrupt to the highest priority */
//    HAL_NVIC_SetPriority(IIC_EXPBD_EV_IRQn, 2, 2);
//    HAL_NVIC_EnableIRQ(IIC_EXPBD_EV_IRQn);

//    /* Enable and set I2C_EXPBD Interrupt to the highest priority */
//    HAL_NVIC_SetPriority(IIC_EXPBD_ER_IRQn, 2, 1);
//    HAL_NVIC_EnableIRQ(IIC_EXPBD_ER_IRQn);
}

/**
 * @brief  Configures I2C interface.
 * @param  None
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t IIC1_Init(void)
{
    if(HAL_I2C_GetState( &I2C1_Handle) == HAL_I2C_STATE_RESET )
    {

      /* I2C_EXPBD peripheral configuration */
      I2C1_Handle.Init.ClockSpeed = IIC_SPEED;
      I2C1_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;


      I2C1_Handle.Init.OwnAddress1    = 0x33;
      I2C1_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
      I2C1_Handle.Instance            = IIC_EXPBD;

      /* Init the I2C */
      IIC1_MspInit();
      HAL_I2C_Init( &I2C1_Handle );
    }

    if( HAL_I2C_GetState( &I2C1_Handle) == HAL_I2C_STATE_READY )
    {
      return 0;
    }
    else
    {
      return 1;
    }
}

/**
 * @brief  Manages error callback by re-initializing I2C
 * @param  Addr I2C Address
 * @retval None
 */
static void I2C_EXPBD_Error( uint8_t Addr )
{
    /* De-initialize the I2C comunication bus */
    HAL_I2C_DeInit( &I2C1_Handle );

    /* Re-Initiaize the I2C comunication bus */
    IIC1_Init();
}


/**
 * @brief  Write data to the register of the device through BUS
 * @param  Addr Device address on BUS
 * @param  Reg The target register address to be written
 * @param  pBuffer The data to be written
 * @param  Size Number of bytes to be written
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t IIC_WriteData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Write( &I2C1_Handle, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, I2C_Timeout );

    /* Check the communication status */
    if( status != HAL_OK )
    {

      /* Execute user timeout callback */
      I2C_EXPBD_Error( Addr );
      return 1;
    }
    else
    {
      return 0;
    }
}



/**
 * @brief  Read a register of the device through BUS
 * @param  Addr Device address on BUS
 * @param  Reg The target register address to read
 * @param  pBuffer The data to be read
 * @param  Size Number of bytes to be read
 * @retval 0 in case of success
 * @retval 1 in case of failure
 */
uint8_t IIC_ReadData( uint8_t Addr, uint8_t Reg, uint8_t* pBuffer, uint16_t Size )
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_I2C_Mem_Read( &I2C1_Handle, Addr, ( uint16_t )Reg, I2C_MEMADD_SIZE_8BIT, pBuffer, Size, I2C_Timeout );

    /* Check the communication status */
    if( status != HAL_OK )
    {

      /* Execute user timeout callback */
      I2C_EXPBD_Error( Addr );
      return 1;
    }
    else
    {
      return 0;
    }
}

