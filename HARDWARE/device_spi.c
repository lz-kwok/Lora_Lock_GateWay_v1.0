/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_spi.c
*作用       : spi设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"


uint8_t SPI_Rxbuffer[9];
SPI_HandleTypeDef SpiHandle1;

//uint8_t SPI1_ReadWriteByte(uint8_t TxData);




/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpioinitstruct;
  
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
    gpioinitstruct.Pull       = GPIO_NOPULL;
    gpioinitstruct.Speed      = GPIO_SPEED_HIGH;
    
    gpioinitstruct.Pin        = GPIO_PIN_5;     //SPI1_SCLK 
    gpioinitstruct.Alternate  = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &gpioinitstruct);

    gpioinitstruct.Pin        = GPIO_PIN_6;     //SPI1_MISO  
    gpioinitstruct.Alternate  = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &gpioinitstruct);

    gpioinitstruct.Alternate  = GPIO_AF5_SPI1;
    gpioinitstruct.Pin        = GPIO_PIN_7;     //SPI1_MOSI  
    HAL_GPIO_Init(GPIOA, &gpioinitstruct);
}


uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&SpiHandle1,&TxData,&Rxdata,1, 1000);       
    
    return Rxdata;          			
}


//硬件SPI1初始化函数,输入形参是分频数，调节速率，分别为：2、4、8、16、32、64、128、256
void SPI1_init(uint16_t speed_set)
{
    uint32_t speed;
    switch(speed_set)
    {
        case 2:
            speed = SPI_BAUDRATEPRESCALER_2;
            break;
        case 4:
            speed = SPI_BAUDRATEPRESCALER_4;
            break;
        case 8:
            speed = SPI_BAUDRATEPRESCALER_8;
            break;
        case 16:
            speed = SPI_BAUDRATEPRESCALER_16;
            break;
        case 32:
            speed = SPI_BAUDRATEPRESCALER_32;
            break;
        case 64:
            speed = SPI_BAUDRATEPRESCALER_64;
            break;
        case 128:
            speed = SPI_BAUDRATEPRESCALER_128;
            break;
        case 256:
            speed = SPI_BAUDRATEPRESCALER_256;
            break;
    }

    HAL_SPI_DeInit(&SpiHandle1);
    
    SpiHandle1.Instance               = SPI1;                        
    SpiHandle1.Init.Mode              = SPI_MODE_MASTER;             
    SpiHandle1.Init.Direction         = SPI_DIRECTION_2LINES;   
    SpiHandle1.Init.DataSize          = SPI_DATASIZE_8BIT;      
    SpiHandle1.Init.CLKPolarity       = SPI_POLARITY_LOW;    
    SpiHandle1.Init.CLKPhase          = SPI_PHASE_1EDGE;         
    SpiHandle1.Init.NSS               = SPI_NSS_SOFT;                 
    SpiHandle1.Init.BaudRatePrescaler = speed;
    SpiHandle1.Init.FirstBit          = SPI_FIRSTBIT_MSB;        
    SpiHandle1.Init.TIMode            = SPI_TIMODE_DISABLE;        
    SpiHandle1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle1.Init.CRCPolynomial     = 7;                  
    HAL_SPI_Init(&SpiHandle1);
    
    __HAL_SPI_ENABLE(&SpiHandle1);  
    
//    HAL_NVIC_SetPriority(SPI1_IRQn, 2, 3);
//    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    
//    if(HAL_SPI_Receive_IT(&SpiHandle1,SPI_Rxbuffer,9)!=HAL_OK)
//      Error_Handler();
    
    SPI1_ReadWriteByte(0xFF);
    
    System.Device.Spi1.WriteReadData = SPI1_ReadWriteByte;
}



