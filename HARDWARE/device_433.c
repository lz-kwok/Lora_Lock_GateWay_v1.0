/*
********************************************************************************
*                                 嵌入式微系统
*                                     msOS
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_433.c
*作用       : 433模组设置
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/10/7         初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"



//FF56AE35A9558C000E069F00040705000005013A3A0503
//FF56AE35A9558B000E
//433.92   7   25K  50MS   38400   无
//频率433.92MHz，空中速率10Kbps，发射功率为最大（12.5dBm），串口速率9600bps，
//无效验,唤醒时间1s，延时触发时间5ms，FEC 使能，本机地址0x01，目标地址0x03，包长
//32，包完整模式开，载波监听关，场强检测开
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
    SetCMDBuf[8]  = 0x03;         //参数长度
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
    SetCMDBuf[8] = 0x01;       //参数长度
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










