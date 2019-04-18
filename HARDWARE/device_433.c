/*
********************************************************************************
*                                 Ƕ��ʽ΢ϵͳ
*                                     msOS
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_433.c
*����       : 433ģ������
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2017/10/7         ��ʼ�汾
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"



//FF56AE35A9558C000E069F00040705000005013A3A0503
//FF56AE35A9558B000E
//433.92   7   25K  50MS   38400   ��
//Ƶ��433.92MHz����������10Kbps�����书��Ϊ���12.5dBm������������9600bps��
//��Ч��,����ʱ��1s����ʱ����ʱ��5ms��FEC ʹ�ܣ�������ַ0x01��Ŀ���ַ0x03������
//32��������ģʽ�����ز������أ���ǿ��⿪
const uint8_t DefaultReg[23] = {0xFF,0x56,0xAE,0x35,0xA9,0x55,0x8C,0x00,0x0E,0x06,0x9F,
0x00,0x04,0x07,0x05,0x00,0x00,0x05,0x01,0x01,0x03,0x05,0x04};

const uint8_t Reset433Module[7] = {0xFF,0x56,0xAE,0x35,0xA9,0x55,0x01};
const uint8_t Read433ParaCMD[9] = {0xFF,0x56,0xAE,0x35,0xA9,0x55,0x8B,0x00,0x0E};

uint8_t SetCMDBuf[14] = {0xFF,0x56,0xAE,0x35,0xA9,0x55,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


void Init433Module(void)
{
    uart1_init(9600);	
    SETA_SET();
    SETB_SET();
    DTUPowerOn();
    delay_ms(200);
    
    System.Device.Usart1.WriteNData((uint8_t *)Read433ParaCMD,9);   
//        System.Device.Usart1.WriteNData((byte *)DefaultReg,23);   

    delay_ms(200);
    
    SETA_CLR();
    SETB_CLR();
    uart1_init(38400);	  
    delay_ms(100);
}

void SetFrequency(uint8_t reg,uint32_t Freq)
{
    static uint8_t kk = 0;
    uart1_init(9600);	
    DTUPowerOff();
    SETA_SET();
    SETB_SET();
    delay_ms(100);
    DTUPowerOn();
    delay_ms(100);
    
    SetCMDBuf[7]  = reg;
    SetCMDBuf[8]  = 0x03;         //��������
    SetCMDBuf[9]  = (unsigned char)((Freq & 0xFFFF00) >> 16);
    SetCMDBuf[10] = (unsigned char)((Freq & 0xFF00) >> 8);
    SetCMDBuf[11] = (unsigned char)(Freq & 0xFF);
    
    for(kk=0;kk<12;kk++)
      System.Device.Usart1.WriteData(SetCMDBuf[kk]);   
    
    delay_ms(200);
    
    uart1_init(38400);	
    DTUPowerOff();
    SETA_CLR();
    SETB_CLR();
    delay_ms(100);
    DTUPowerOn();
}


void SetSlaverAddr(uint8_t reg,uint8_t Addr)
{
    static uint8_t kk = 0;
    uart1_init(9600);	
    DTUPowerOff();
    SETA_SET();
    SETB_SET();
    delay_ms(100);
    DTUPowerOn();
    delay_ms(100);
    
    SetCMDBuf[7] = reg;
    SetCMDBuf[8] = 0x01;       //��������
    SetCMDBuf[9] = Addr;
    
    for(kk=0;kk<10;kk++)
      System.Device.Usart1.WriteData(SetCMDBuf[kk]);   
    
    delay_ms(200);
    uart1_init(38400);	
    DTUPowerOff();
    SETA_CLR();
    SETB_CLR();
    delay_ms(100);
    DTUPowerOn();
}










