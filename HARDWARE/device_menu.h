/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_menu.h
*作用       : ui业务逻辑
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_MENU__H
#define __DEVICE_MENU__H

#include "config.h"
/* Includes ------------------------------------------------------------------*/

#define CodeValue      131

#define Homepage       0
#define Menupage       1
#define Recvpage       2
#define Parapage       3
#define Langpage       4
#define Systpage       5
#define Exitpage       6
#define Poffpage       7

extern WINDOWS winxp;


extern void HomePageShow(void);
extern void SetupPageShow(int8_t ctr);
extern void SystemSetPageShow(int8_t ctr);
extern void ParameterSetShow(int8_t ctr);
extern void ParameterSetupMenu(uint8_t ctr);
extern void SelectReceiverMenuShow(void);
extern void SelectFrebandMenuShow(void);
extern void CipherCheckFunction(uint8_t KeyIndex);
extern void ParaSetFunction(uint8_t ctr,uint8_t KeyIndex);
extern void PoweroffMenu(void);
extern void SystemSetupMenu(int8_t ctr);
extern void SysSetFunction(uint8_t ctr,uint8_t KeyIndex);
extern void LanguageSetMenu(void);
extern void ParameterInqury(void* parameter);
extern void DelayTurnoff(void* parameter);

void thread_Menu_show_entry(void* parameter);



#endif /* __DEVICE_MENU__H */
