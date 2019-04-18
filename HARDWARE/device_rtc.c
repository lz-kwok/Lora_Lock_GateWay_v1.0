/*
********************************************************************************
*                            �й���������������ϵͳ
*                            Thread Operating System
*
*                              ��оƬ:STM32F401re
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_rtc.c
*����       : ��ʼ���ڲ�ʵʱʱ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/12/13        ��ʼ�汾
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"

#define RTC_CLOCK_SOURCE_LSE



#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0xF9
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

RTC_HandleTypeDef RtcHandle; 


void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/
#ifdef RTC_CLOCK_SOURCE_LSE
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
#elif defined (RTC_CLOCK_SOURCE_LSI)
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
#else
#error Please select the RTC Clock source inside the main.h file
#endif /*RTC_CLOCK_SOURCE_LSE*/
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
  
//  /*##-3- Configure the NVIC for RTC TimeStamp ###################################*/
//  HAL_NVIC_SetPriority(TAMP_STAMP_IRQn, 0x0F, 0);
//  HAL_NVIC_EnableIRQ(TAMP_STAMP_IRQn);
}

#ifdef SOC_RTC
/**
 * @brief  Configures the RTC
 * @param  None
 * @retval None
 */
void Rtc_Init(void)
{
  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
//  HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR1,0x32F2);
  
//   RTC_Wakeup_Enable(1);    //���������ж�
//   RTC_Alarm_Set();   //���������ж�
}


//���������� �����ж�
void RTC_Alarm_Set(void)
{
  RTC_TimeTypeDef stimestructure;
  RTC_AlarmTypeDef  RTC_AlarmStructure;
//  RTC_DateTypeDef sdatestructureget;

//�Զ���ʱ��ѡ��FORMAT_BIN   ʹ��RTC��ȡʱ��ѡ��FORMAT_BCD 
  HAL_RTC_GetTime(&RtcHandle, &stimestructure, FORMAT_BCD);    
//  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);
  
//  RTC_AlarmStructure.AlarmTime.Hours   = 0x14;
//  RTC_AlarmStructure.AlarmTime.Minutes = 0x59;
//  RTC_AlarmStructure.AlarmTime.Seconds = 0x50;
  RTC_AlarmStructure.AlarmTime.Hours   = stimestructure.Hours;
  RTC_AlarmStructure.AlarmTime.Minutes = stimestructure.Minutes + 1;
  RTC_AlarmStructure.AlarmTime.Seconds = stimestructure.Seconds;
  RTC_AlarmStructure.AlarmTime.SubSeconds = 0x00;
  
  RTC_AlarmStructure.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  RTC_AlarmStructure.AlarmTime.StoreOperation= RTC_STOREOPERATION_RESET;
  RTC_AlarmStructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;           //�������ں����ڱ�������
//  RTC_AlarmStructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
//  RTC_AlarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;    //�����ڡ�������   
//  RTC_AlarmStructure.AlarmDateWeekDay = sdatestructureget.Date;            //�������ڡ�����
  
  if(RTC_AlarmStructure.AlarmTime.Seconds == 60)
    RTC_AlarmStructure.AlarmTime.Seconds=0;
  
  RTC_AlarmStructure.Alarm = RTC_ALARM_A;
  
  if(HAL_RTC_SetAlarm_IT(&RtcHandle,&RTC_AlarmStructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}


//�������������ڻ����жϣ����жϣ�
void RTC_Wakeup_Enable(uint16_t second)            
{
  static uint16_t time;
  time = second - 1;
  //Enable the WakeUp
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, time, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);//
  RtcHandle.Instance->ISR = 0x00000000;     // ��һ��ISR�Ĵ���
  
  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 1, 3);
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
//�ر����ڻ����ж�
void RTC_Wakeup_Disable(void ) 
{
   HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
}



void GetRtc1000Routine(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructure;
  
  HAL_RTC_GetTime(&RtcHandle, &stimestructure, FORMAT_BIN);
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);
  
  if(AppDataPointer->Rtc.Day != sdatestructureget.Date)
  {
//    NB_Seqnomuber = 0;                           //ÿ��00:00:00����
  }
  AppDataPointer->Rtc.Year   = sdatestructureget.Year;
  AppDataPointer->Rtc.Month  = sdatestructureget.Month;
  AppDataPointer->Rtc.Day    = sdatestructureget.Date;
  AppDataPointer->Rtc.Hour   = stimestructure.Hours;
  AppDataPointer->Rtc.Minute = stimestructure.Minutes;
  AppDataPointer->Rtc.Second = stimestructure.Seconds;
  
  if(stimestructure.Minutes % 5 == 0)          //ÿ5���ӽ���Ϊ��һ���ӵĲɼ�
  {
		;
  }
  else
  {
		;
  }

}  

  //����ʱ���
//void CreateTime(void)
//{
//    StorageBuf[0] = (AppDataPointer->Rtc.Hour/10)+0x30;	
//    StorageBuf[1] = (AppDataPointer->Rtc.Hour%10)+0x30;	
//    StorageBuf[3] = (AppDataPointer->Rtc.Minute/10)+0x30;
//    StorageBuf[4] = (AppDataPointer->Rtc.Minute%10)+0x30;
//    StorageBuf[6] = (AppDataPointer->Rtc.Second/10)+0x30;
//    StorageBuf[7] = (AppDataPointer->Rtc.Second%10)+0x30; 
//    
//    System.Device.UsbVcp.WriteData(StorageBuf,8);
//    System.Device.UsbVcp.WriteString("\r\n");
////    System.Device.Timer.Start(3,TimerSystick,5000,CreateTime);
//}



void SyncRTCRegulate(uint8_t *data)
{
  RTC_TimeTypeDef stimestructure;
  RTC_DateTypeDef sdatestructureget;

  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.Hours      = data[4];
  stimestructure.Minutes    = data[5];
  stimestructure.Seconds    = data[6];
  stimestructure.SubSeconds = 0;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  sdatestructureget.Year    = data[1];
  sdatestructureget.Month   = data[2];
  sdatestructureget.Date    = data[3];
  sdatestructureget.WeekDay = data[7];
  
  if(HAL_RTC_SetDate(&RtcHandle, &sdatestructureget, FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


//void CreateFileName(void)
//{
//    //�ļ����ϳ�
//    StoreFile[5] = (AppDataPointer->Rtc.Year/10)+0x30;
//    StoreFile[6] = (AppDataPointer->Rtc.Year%10)+0x30;		
//    StoreFile[7] = (AppDataPointer->Rtc.Month/10)+0x30;
//    StoreFile[8] = (AppDataPointer->Rtc.Month%10)+0x30;
//    StoreFile[9] = (AppDataPointer->Rtc.Day/10)+0x30;
//    StoreFile[10] = (AppDataPointer->Rtc.Day%10)+0x30;
//}




////RTC����A�жϷ�����
//void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *RtcHandle)
//{
//    RTC_Alarm_Set();
//    System.Device.UsbVcp.WriteString("Alram\r\n");
//}


////RTC�����жϷ�����
//void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *RtcHandle)
//{
//  System.Device.UsbVcp.WriteString("wake up\r\n");

//}


////RTC�����ж�
//void RTC_WKUP_IRQHandler(void)
//{
//  HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
//}


////RTC�����ж�
//void RTC_Alarm_IRQHandler(void)
//{
//  HAL_RTC_AlarmIRQHandler(&RtcHandle);
//}


#endif
