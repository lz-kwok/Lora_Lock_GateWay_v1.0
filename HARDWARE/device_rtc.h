/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
*                               移植/裁剪：郭良志
********************************************************************************
*文件名     : device_rtc.h
*作用       : 初始化内部实时时钟
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/9/20        初始版本
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_RTC__H
#define __DEVICE_RTC__H

/* Includes ------------------------------------------------------------------*/
#include "system.h"


void Rtc_Init(void);
void RTC_Alarm_Set(void);
void InitRtcTimeStamp(void);
void GetRtc1000Routine(void);
void SyncRTCRegulate(uint8_t *data);
void CreateFileName(void);
void CreateTime(void);
void RTC_Wakeup_Enable(uint16_t second);
void RTC_Wakeup_Disable(void );


#endif /* __DEVICEUART__H */
