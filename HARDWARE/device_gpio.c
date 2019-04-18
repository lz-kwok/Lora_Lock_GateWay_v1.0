/*
********************************************************************************
*                            �й���������������ϵͳ
*                            Thread Operating System
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_gpio.c
*����       : IO����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2019/4/5        ��ʼ�汾
********************************************************************************
*/
#include "device_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 


//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��
void Gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    EnablePS_StaGPIO();
    
    /* Configure the GPIO_INPUT pin */
    GPIO_InitStruct.Pin   = StaPin;           //��ʼ��PA10Ϊ��������	
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  //��������Ӧ״̬(������Ӧʱ����ߵ�ƽ�ź�)
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
