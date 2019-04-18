/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : systemglobal.h
*作用       : 系统全局变量
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2019/4/5        初始版本
********************************************************************************
*/


#ifndef __SYSTEMGLOBAL_H
#define __SYSTEMGLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "system.h"

#define uart_rx_len			512

extern UART_HandleTypeDef Uart1Handle;
extern UART_HandleTypeDef Uart2Handle;
extern UART_HandleTypeDef Uart6Handle;
extern DMA_HandleTypeDef hdma_tx;
extern DMA_HandleTypeDef hdma_rx;
extern uint8_t rx2Buff[uart_rx_len];
extern uint8_t rx1Buff[10];

extern uint8_t CmdShow[25];
extern uint8_t ModbusCmd1[8];
extern uint8_t ModbusCmd2[8];
extern uint8_t ModbusCmd3[8];
extern uint8_t ModbusCmd4[8];
extern uint8_t ModbusCmd5[8];
extern uint8_t ModbusCmd6[8];
extern uint8_t ModbusCmd7[8];

extern uint8_t PowerOnFlag;
extern uint8_t Menuhierarchy;
extern int8_t MenuChooseIndex;
extern int8_t ParaChooseIndex;
extern uint8_t ParaMenuFlag;
extern uint8_t ParaAskFlag;
extern uint8_t SystemSetFlag;
extern int8_t SysChooseIndex;
extern int8_t OKorCancel;
extern uint8_t ParaSetConfirm;
extern uint8_t SystemSetConfirm;
extern uint8_t LanguageConfirm;
extern uint8_t DevIDConfirm;
extern uint8_t ChargingFlag;
extern uint8_t Ack_Flag;
extern uint16_t Uart2lenth;
extern uint8_t SignalPower;
extern uint8_t RecPower;
extern int8_t DevIdChooseIndex;
extern int8_t LanSetIndex;
extern int8_t DeviceNum;
extern uint8_t FouthMenuIndex;
extern int8_t FreSetIndex;
extern uint16_t TurnoffTick;
extern int8_t ForceSetIndex;
extern int16_t MovementSetIndex;
extern int8_t InOutIndex;
extern uint8_t InOutValue;
extern int16_t DelaySetIndex;
extern int16_t SpacingIndex;
extern uint16_t OutNumIndex;
extern int16_t LengthSetIndex;
extern int8_t ABSetIndex;
extern uint8_t ABValue;
extern int8_t Shortcut;
extern uint8_t DeviceIDConfirmTick[8];
extern uint8_t SysConfirmTick[8];
extern uint8_t ParaConfirmTick[8];
extern uint32_t CodeSum;
extern uint8_t TypeCodeTick ;
extern uint16_t AckValue;
extern uint8_t RssiFlag;
extern uint8_t NonAck;
extern uint8_t Read433Para;
extern uint8_t TurnoffIndex;
extern uint16_t TurnoffValue;
extern uint8_t resetConfirm;
extern uint8_t writeConfirm;

extern const uint8_t BAT_RB5[];
extern const uint8_t BAT_RB4[];
extern const uint8_t BAT_RB3[];
extern const uint8_t BAT_RB2[];
extern const uint8_t BAT_RB1[];
extern const uint8_t BAT_RB0[];
extern const uint8_t Signal_2[];
extern const uint8_t Signal_1[];
extern const uint8_t Signal_0[];
extern const uint8_t BAT_B3[];
extern const uint8_t BAT_B2[];
extern const uint8_t BAT_B1[];
extern const uint8_t BAT_B0[];
extern const uint8_t BAT_CHAR[];
extern const uint8_t TypeCode[];
extern const uint8_t TypeCodeNone[];
extern const uint8_t NowFrequeceIco[];
extern const uint8_t ParaSetIco[];
extern const uint8_t LanguIco[];
extern const uint8_t ExitSetupIco[];
extern const uint8_t SystemSetIco[];
extern const uint8_t ChooseDevIco[];
extern const uint8_t BackLightIco[];
extern const uint8_t BackLightTimeIco[];
extern const uint8_t TurnoffTimeIco[];
extern const uint8_t IDIco[];
extern const uint8_t ForceIco[];
extern const uint8_t InoutIco[];
extern const uint8_t SpacingIco[];
extern const uint8_t LengthIco[];
extern const uint8_t MovementIco[];
extern const uint8_t IndelayIco[];
extern const uint8_t OutnumIco[];
extern const uint8_t ABIco[];
extern const uint8_t ExitIco[];
extern const uint8_t PoweroffIco[];
extern const uint8_t PowerOffTimeIco[];
extern const uint8_t PowerOffTimeIco1[];
extern const uint8_t AlwaysOnIco[];
extern const uint8_t ChineseIco[];
extern const uint8_t EnglishIco[];

extern uint8_t MultiButtonValue[4];
extern uint8_t MultiKey;
extern uint8_t TransmitFail;
extern uint8_t Enableupdate;
extern uint8_t EnableRead;
extern int8_t LanguageIndex;

extern uint16_t CalcuResult;
extern uint8_t CRC_Result[2];
extern uint8_t BatFlag;
extern uint32_t Lock_Code;



#ifdef __cplusplus
}
#endif



#endif 
