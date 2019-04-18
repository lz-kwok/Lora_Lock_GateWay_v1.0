/*
********************************************************************************
*                            �й���������������ϵͳ
*                            Thread Operating System
*
*                              ��оƬ:STM32F401re
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_rtc.h
*����       : ��ʼ���ڲ�ʵʱʱ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2017/9/20        ��ʼ�汾
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
