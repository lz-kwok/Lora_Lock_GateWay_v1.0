/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_adc.c
*作用       : 模数转换
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"

#define ChanSum         5       // 4通道


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handler;     //ADC句柄;


static uint16_t Adc[ChanSum-1];
uint32_t ADSum = 0;
uint8_t ADTick = 0;



void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_ADC1_CLK_ENABLE();
    GPIO_InitStruct.Pin  = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
//    HAL_NVIC_SetPriority(ADC_IRQn, 1, 4);
//    HAL_NVIC_EnableIRQ(ADC_IRQn);
}



/*******************************************************************************
* 函数名    : InitAdc
* 描述	    : ADC初始化
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void Adc_init(void)
{
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=72/4=18MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE; //使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;       		   //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                           		   //初始化                               
}


uint16_t GetAdcValue(uint8_t ch)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;            //通道
    ADC1_ChanConf.Rank=1;                //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES; //采样时间
    ADC1_ChanConf.Offset=0;
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf); //通道配置
    HAL_ADC_Start(&ADC1_Handler);                        //开启AD
    HAL_ADC_PollForConversion(&ADC1_Handler,10);         //轮询转换
    
    return (uint16_t)HAL_ADC_GetValue(&ADC1_Handler);  //返回最近转换结果
}




/*******************************************************************************
* 描述	    : 系统时钟100/S，即10mS间隔调用一次运行，获取ADC值给App层变量
*******************************************************************************/
void AdcSystick100Routine(void)
{
    Adc[0] = GetAdcValue(0);
    
    ADSum += Adc[0];
    ADTick ++;
    
    if(ADTick == 100)
    {
        ADTick = 0;
        ADSum = 0;
        if(ADSum > 180000)
        {
			AppDataPointer->Adc.A0 = ADSum / 100;
        }
    }
}




