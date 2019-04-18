/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : data.h
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz        2016/09/13        初始版本
********************************************************************************
*/


#ifndef __DATA_H
#define __DATA_H

#include "system.h"


typedef struct
{
    uint32_t Second;
    uint32_t Minute;
    uint32_t Hour;
    uint32_t Day;
    uint32_t Week;
    uint32_t Month;
    uint32_t Year;
}RtcStruct;

typedef struct
{
    float Temprature;
    float Humidity;
    float Press;
}BMEStruct;


typedef enum
{
    AdcChanne0 = 0,
    AdcChanne1 = 1,
    AdcChanne2 = 2,
    AdcChanne3 = 3        
}AdcChannelEnum;


typedef struct 
{
    uint16_t A0;
    uint16_t A1;
    uint16_t A2;
    uint16_t A3;
    uint16_t * pA0;
    uint16_t * pA1;
    uint16_t * pA2;
    uint16_t * pA3;
}AdcStruct;


typedef struct
{
    char Voltage;

    BMEStruct Bme;
    RtcStruct Rtc;

    AdcStruct Adc;
}DataStruct;

typedef struct
{
    DataStruct Data;
}AppStruct;











#endif /*__Data_H*/

