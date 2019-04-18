/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*                              ��оƬ:STM32F411re/cc
*                           �����ϴ���ά���ӿƼ����޹�˾
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_433.h
*����       : 433ģ������
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2017/10/7        ��ʼ�汾
********************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_433__H
#define __DEVICE_433__H

extern const uint8_t Read433ParaCMD[9];

#define SlaverAddr             0x0C

/* Includes ------------------------------------------------------------------*/
#define FreqReg                0x00
#define AirSpeedReg            0x03
#define TransPowerReg          0x04
#define UartbaudReg            0x05
#define UartParityReg          0x06
#define WakeTimeReg            0x07
#define TrigTimeReg            0x08
#define Function1Reg           0x09
#define MasterAddrReg          0x0A
#define SlaverAddrReg          0x0B
#define DataLengthReg          0x0C
#define Function2Reg           0x0D

//-----------Ƶ�ʺ궨��--------------------------------
#define CH1                    0x69F00           //433.920Mhz
#define CH2                    0x69B18           //432.920Mhz
#define CH3                    0x69730           //431.920Mhz
#define CH4                    0x6A2E8           //434.920Mhz
#define CH5                    0x6A6D0           //435.920Mhz
#define CH6                    0x6AAB8           //436.920Mhz
#define CH7                    0x6AEA0           //437.920Mhz
#define CH8                    0x6B288           //438.920Mhz



void Init433Module(void);
void SetFrequency(uint8_t reg,uint32_t Freq);
void SetSlaverAddr(uint8_t reg,uint8_t Addr);











#endif /* __DEVICE_433__H*/
