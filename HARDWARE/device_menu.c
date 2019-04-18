/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_menu.c
*作用       : UI业务逻辑
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/


/* Includes ------------------------------------------------------------------*/
#include "system.h"

struct rt_timer soft_timer5;


char mulString1[20] = "盘点:";
char mulString2[20] = "存盘:";
char mulString3[20] = "取盘:";
char mulString4[20] = "总数:";
char mulString5[10] = "";


extern uint8_t OLED_GRAM[128][8];
extern char cacheBuf[10];

uint8_t CipherLineNum = 95;
WINDOWS winxp;
WINDOWS HomePage;

void HomepageParsing(uint16_t ack)
{
    static uint16_t minusData;

    switch(ack)
    {
      case 0:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"待机状态";
        break;
      case 1:
        HomePage.Text.x = 56;
        HomePage.Text.content = (uint8_t *)"UP";
        break;
      case 2:
        HomePage.Text.x = 56;
        HomePage.Text.content = (uint8_t *)"DN";
        break;
      case 3:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"FWD L";
        break;
      case 4:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"FWD H";
        break;
      case 5:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"BKW L";
        break;
      case 6:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"BKW H";
        break;
      case 7:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"单出中";
        break;
      case 8:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"单出完成";
        break;
      case 9:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"单入中";
        break;
      case 10:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"单入完成";
        break;
      case 11:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"连入中";
        break;
      case 12:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"连入完成";
        break;
      case 13:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"连出中";
        break;
      case 14:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"连出完成";
        break;
      case 15:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"盘点中";
        break;
      case 16:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"整理中";
        break;
      case 31:
        HomePage.Text.x = 28;
        HomePage.Text.content = (uint8_t *)"远程电量低";
        break;
      case 32:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"前障碍";
        break;
      case 33:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"后障碍";
        break;
      case 34:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"UP-DN超时";
        break;
      case 35:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"前后超时";
        break;
      case 36:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"前减PLS故障";
        break;
      case 37:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"后减PLS故障"; 
        break;
      case 38:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"前限PLS故障";  
        break;
      case 39:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"后限PLS故障";  
        break;
      case 40:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"前减PLS故障";
        break;
      case 41:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"后减PLS故障"; 
        break;
      case 42:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"前盘PLS故障"; 
        break;
      case 43:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"后盘PLS故障"; 
        break;
      case 44:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"前位PLS故障"; 
        break;
      case 45:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"后位PLS故障"; 
        break;
      case 46:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"前障PLS故障"; 
        break;
      case 47:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"后障PLS故障"; 
        break;
      case 48:
        HomePage.Text.x = 24;
        HomePage.Text.content = (uint8_t *)"接近PLS故障"; 
        break;
      case 49:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"前后驱动故障"; 
        break;
      case 50:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"编码故障"; 
        break;
      case 51:
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)"脱轨故障"; 
        break;
      case 52:
        HomePage.Text.x = 20;
        HomePage.Text.content = (uint8_t *)"升降驱动故障"; 
        break;
      case 53:
        HomePage.Text.x = 40;
        HomePage.Text.content = (uint8_t *)"盘超长"; 
        break;
      default:            // ???,??????
        HomePage.Text.x = 56;
        sprintf(&mulString5[0],"%d",ack);
        
        HomePage.Text.content = (uint8_t *)mulString5; 
        break;
    }
    
    
    if((ack>999)&&(ack<2000))
    {
        minusData = ack - 1000;
        sprintf(&mulString1[5],"%d",minusData);
        
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)mulString1; 
    }
    else if((ack>1999)&&(ack<3000))
    {
        minusData = ack - 2000;
        sprintf(&mulString2[5],"%d",minusData);
        
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)mulString2;
    }
    else if((ack>2999)&&(ack<4000))
    {
        minusData = ack - 3000;
        sprintf(&mulString3[5],"%d",minusData);
        
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)mulString3;
    }
    else if((ack>3999)&&(ack<5000))
    {
        minusData = ack - 4000;
        sprintf(&mulString4[5],"%d",minusData);
        
        HomePage.Text.x = 32;
        HomePage.Text.content = (uint8_t *)mulString4;
    }
}


static void SetupWindow(uint8_t *title)
{
    winxp.x = 2;
    winxp.y = 0;
    winxp.with = 126;
    winxp.hight = 64;
    winxp.title = title;
    winxp.state = NULL;
      
    GUI_WindowsDraw(&winxp);	
}

static void HomeWindow(void)
{
    HomePage.x = 2;
    HomePage.y = 0;
    HomePage.with = 126;
    HomePage.hight = 64;
    
    if(GetBit(Menuhierarchy,0) == 1)              //??????????,????
    {
      EnableRead = 0;
      HomePage.Text.content = NULL;
      switch(TypeCodeTick)
      {
        case 0:
          HomePage.title = "TYPE CODE:";
          break;
        case 1:
          HomePage.title = "TYPE CODE: *";
          break;
        case 2:
          HomePage.title = "TYPE CODE: **";
          break;
      }
    }
    else
    {
      EnableRead = 1;
//      HomePage.title = "HOME";
      HomePage.Text.y = 5;    
      
      if(Enableupdate == 1)
      {
        Enableupdate = 0;
        
        HomepageParsing(AckValue);
      }
      else
      {
        if(NonAck)
        {
            HomePage.Text.x = 32;
            HomePage.Text.content = (uint8_t *)"通讯失败";
        }
//        else
//        {
//            HomePage.Text.x = 8;
////            HomePage.Text.content = null;
//        }
      }
    }
    
    HomePage.state = NULL;


    GUI_WindowsDraw(&HomePage);
}



void HomePageShow(void)
{
  HomeWindow();
    
  GUI_PutString(106,15,"B");
  GUI_PutString(50,15,"RB");      //????
  
  if(InOutValue == 1)
    GUI_PutString(50,25,"F0");
  else
    GUI_PutString(50,25,"L0");
  
  if(ABValue == 1)
    GUI_PutString(106,25,"A<");
  else
    GUI_PutString(106,25,"<B");

  switch(DevIdChooseIndex)      //??????
  {
    case 1:
        GUI_PutString(17,25,"1#");
      break;
    case 2:
        GUI_PutString(17,25,"2#");
      break;
    case 3:
        GUI_PutString(17,25,"3#");
      break;
    case 4:
        GUI_PutString(17,25,"4#");
      break;
    case 5:
        GUI_PutString(17,25,"5#");
      break;
    case 6:
        GUI_PutString(17,25,"6#");
      break;
    case 7:
        GUI_PutString(17,25,"7#");
      break;
    case 8:
        GUI_PutString(17,25,"8#");
      break;
  }

  if(FreSetIndex < 9)
  {
    switch(FreSetIndex)      //??????
    {
      case 1:
        GUI_PutString(17,15,"CH1");
        break;
      case 2:
        GUI_PutString(17,15,"CH2");
        break;
      case 3:
        GUI_PutString(17,15,"CH3");
        break;
      case 4:
        GUI_PutString(17,15,"CH4");
        break;
      case 5:
        GUI_PutString(17,15,"CH5");
        break;
      case 6:
        GUI_PutString(17,15,"CH6");
        break;
      case 7:
        GUI_PutString(17,15,"CH7");
        break;
      case 8:
        GUI_PutString(17,15,"CH8");
        break;
    }
  }
  else
    GUI_PutString(17,15,"CH1");
  
  if(NonAck)
    GUI_LoadPic1(4,15,(uint8_t *)Signal_0,8,8);
  else
  {
    switch(SignalPower)       //??????
    {
      case 0:
        GUI_LoadPic1(4,15,(uint8_t *)Signal_0,8,8);
        break;
      case 1:
        GUI_LoadPic1(4,15,(uint8_t *)Signal_1,10,8);
        break;
      case 2:
        GUI_LoadPic1(4,15,(uint8_t *)Signal_2,10,8);
        break;
    }
  }
  
  if(RecPower < 6)
  {
    switch(RecPower)                             //???????
    {
      case 0:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB0,19,7);
        break;
      case 1:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB5,19,7);
        break;
      case 2:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB4,19,7);
        break;
      case 3:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB3,19,7);
        break;
      case 4:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB2,19,7);
        break;
      case 5:
        GUI_LoadPic1(64,15,(uint8_t *)BAT_RB1,19,7);
        break;
    }
  }
  else
    GUI_LoadPic1(64,15,(uint8_t *)BAT_RB3,19,7);
  

  if(ChargingFlag == 1)                         //???????
    GUI_LoadPic1(113,15,(uint8_t *)BAT_CHAR,13,7);
  else
  {
    if(AppDataPointer->Adc.A0 > 2500)           //??????
      GUI_LoadPic1(113,15,(uint8_t *)BAT_B3,13,7);
    else if((AppDataPointer->Adc.A0 > 2250)&&(AppDataPointer->Adc.A0 <= 2500))
      GUI_LoadPic1(113,15,(uint8_t *)BAT_B2,13,7);
    else if((AppDataPointer->Adc.A0 > 2050)&&(AppDataPointer->Adc.A0 <= 2250))
      GUI_LoadPic1(113,15,(uint8_t *)BAT_B1,13,7);
    else if(AppDataPointer->Adc.A0 <= 2050)
      GUI_LoadPic1(113,15,(uint8_t *)BAT_B0,13,7);
  }
}

/*******************************************************************************
* ???    : SetupPageShow
* ??	    : ??????
* ????  : ?
* ????  : ?
*******************************************************************************/
void SetupPageShow(int8_t ctr)
{
   SetupWindow("Setup");
   
   if(ctr == 0)
     GUI_LoadPic(8,15,(uint8_t *)ChooseDevIco,56,16);
   else
     GUI_LoadPic1(8,15,(uint8_t *)ChooseDevIco,56,16);
   
   if(ctr == 1)
     GUI_LoadPic(8,31,(uint8_t *)ParaSetIco,56,16);
   else
     GUI_LoadPic1(8,31,(uint8_t *)ParaSetIco,56,16);
   
   if(ctr == 2)
     GUI_LoadPic(8,47,(uint8_t *)LanguIco,56,16);
   else
     GUI_LoadPic1(8,47,(uint8_t *)LanguIco,56,16);
   
   if(ctr == 3)
     GUI_LoadPic(66,15,(uint8_t *)SystemSetIco,56,16);
   else
     GUI_LoadPic1(66,15,(uint8_t *)SystemSetIco,56,16);
   
   if(ctr == 4)
     GUI_LoadPic(66,31,(uint8_t *)ExitSetupIco,56,16);
   else
     GUI_LoadPic1(66,31,(uint8_t *)ExitSetupIco,56,16);
}

void SystemSetPageShow(int8_t ctr)
{
   SetupWindow("System");
   
   if(ctr == 0)
     GUI_LoadPic(8,15,(uint8_t *)NowFrequeceIco,56,16);
   else
     GUI_LoadPic1(8,15,(uint8_t *)NowFrequeceIco,56,16);
   
   if(ctr == 1)
     GUI_LoadPic(8,31,(uint8_t *)BackLightIco,56,16);
   else
     GUI_LoadPic1(8,31,(uint8_t *)BackLightIco,56,16);
   
   if(ctr == 2)
     GUI_LoadPic(8,47,(uint8_t *)BackLightTimeIco,56,16);
   else
     GUI_LoadPic1(8,47,(uint8_t *)BackLightTimeIco,56,16);

   if(ctr == 3)
     GUI_LoadPic(66,15,(uint8_t *)TurnoffTimeIco,56,16);
   else
     GUI_LoadPic1(66,15,(uint8_t *)TurnoffTimeIco,56,16);

   if(ctr == 4)
     GUI_LoadPic(66,31,(uint8_t *)IDIco,56,16);
   else
     GUI_LoadPic1(66,31,(uint8_t *)IDIco,56,16);

   if(ctr == 5)
     GUI_LoadPic(66,47,(uint8_t *)ExitSetupIco,56,16);
   else
     GUI_LoadPic1(66,47,(uint8_t *)ExitSetupIco,56,16);
}



void ParameterSetShow(int8_t ctr)
{
   SetupWindow("Parameter");
   
   if(ctr == 0)
     GUI_LoadPic(8,15,(uint8_t *)ForceIco,28,16);
   else
     GUI_LoadPic1(8,15,(uint8_t *)ForceIco,28,16);
   
   if(ctr == 1)
     GUI_LoadPic(36,15,(uint8_t *)InoutIco,28,16);
   else
     GUI_LoadPic1(36,15,(uint8_t *)InoutIco,28,16);
   
   if(ctr == 2)
     GUI_LoadPic(64,15,(uint8_t *)SpacingIco,28,16);
   else
     GUI_LoadPic1(64,15,(uint8_t *)SpacingIco,28,16);
   
   if(ctr == 3)
     GUI_LoadPic(92,15,(uint8_t *)LengthIco,28,16);
   else
     GUI_LoadPic1(92,15,(uint8_t *)LengthIco,28,16);
   
   if(ctr == 4)
     GUI_LoadPic(4,31,(uint8_t *)MovementIco,42,16);
   else
     GUI_LoadPic1(4,31,(uint8_t *)MovementIco,42,16);
 
   if(ctr == 5)
     GUI_LoadPic(46,31,(uint8_t *)IndelayIco,42,16);
   else
     GUI_LoadPic1(46,31,(uint8_t *)IndelayIco,42,16);
   
   if(ctr == 6)
     GUI_LoadPic(4,47,(uint8_t *)OutnumIco,42,16);
   else
     GUI_LoadPic1(4,47,(uint8_t *)OutnumIco,42,16);

   if(ctr == 7)
     GUI_LoadPic(46,47,(uint8_t *)ABIco,42,16);
   else
     GUI_LoadPic1(46,47,(uint8_t *)ABIco,42,16);

   if(ctr == 8)
     GUI_LoadPic(92,47,(uint8_t *)ExitIco,28,16);
   else
     GUI_LoadPic1(92,47,(uint8_t *)ExitIco,28,16);
   
}


static void ForceMenuShow()
{
   SetupWindow("Force:0 or 1");
   GUI_LoadPic1(8,15,(uint8_t *)ForceIco,28,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",ForceSetIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40002,0x06,0x00,ForceSetIndex);

        rt_timer_init(&soft_timer5, "ParameterInqury",       
                          ParameterInqury,                          
                          RT_NULL,                                   
                          500,                                          
                          RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void InoutMenuShow()
{
   if(InOutValue == 1)
     SetupWindow("Inout:1 is F0");
   else if(InOutValue == 0)
     SetupWindow("Inout:1 is L0");
     
   GUI_LoadPic1(8,15,(uint8_t *)InoutIco,28,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",InOutIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40003,0x06,0x00,InOutIndex);
        InOutValue = InOutIndex;
        FlashRsvWrite(&InOutValue,2,1);

        rt_timer_init(&soft_timer5, "ParameterInqury",       
                  ParameterInqury,                          
                  RT_NULL,                                   
                  500,                                          
                  RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void SpacingMenuShow(void)
{
   static uint8_t Spacing_H;
   static uint8_t Spacing_L;
   SetupWindow("Spacing:0-500");
   GUI_LoadPic1(8,15,(uint8_t *)SpacingIco,28,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",SpacingIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        Spacing_H = (uint8_t)((SpacingIndex&0xff00)>>8);
        Spacing_L = (uint8_t)(SpacingIndex&0xFF);
        ModbusMasterAsk(SlaverAddr,40004,0x06,Spacing_H,Spacing_L);

        rt_timer_init(&soft_timer5, "ParameterInqury",       
                  ParameterInqury,                          
                  RT_NULL,                                   
                  500,                                          
                  RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void lengthMenuShow(void)
{
   static uint8_t Length_H;
   static uint8_t Length_L;
   SetupWindow("Length");
   GUI_LoadPic1(8,15,(uint8_t *)LengthIco,28,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",LengthSetIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        if(LengthSetIndex < 150)
          LengthSetIndex = 150;
        
        ParaSetConfirm = 1;
        Length_H = (uint8_t)((LengthSetIndex&0xff00)>>8);
        Length_L = (uint8_t)(LengthSetIndex&0xFF);
        ModbusMasterAsk(SlaverAddr,40005,0x06,Length_H,Length_L);

        rt_timer_init(&soft_timer5, "ParameterInqury",       
                  ParameterInqury,                          
                  RT_NULL,                                   
                  500,                                          
                  RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void MovementMenuShow(void)
{
   SetupWindow("Movement");
   GUI_LoadPic1(8,15,(uint8_t *)MovementIco,42,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",MovementSetIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40006,0x06,0x00,MovementSetIndex);

        rt_timer_init(&soft_timer5, "ParameterInqury",       
                  ParameterInqury,                          
                  RT_NULL,                                   
                  500,                                          
                  RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void IndelayMenuShow()
{
   SetupWindow("Indelay Senconds");
   GUI_LoadPic1(8,15,(uint8_t *)IndelayIco,42,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",DelaySetIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40007,0x06,0x00,DelaySetIndex);
        rt_timer_init(&soft_timer5, "ParameterInqury",       
                        ParameterInqury,                          
                        RT_NULL,                                   
                        500,                                          
                        RT_TIMER_FLAG_ONE_SHOT);                     
              
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void OutnumMenuShow(void)
{
   SetupWindow("Out Number");
   GUI_LoadPic1(8,15,(uint8_t *)OutnumIco,42,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",OutNumIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40030,0x06,0x00,OutNumIndex);
        rt_timer_init(&soft_timer5, "ParameterInqury",       
                  ParameterInqury,                          
                  RT_NULL,                                   
                  500,                                          
                  RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

static void ABMenuShow(void)
{
   if(ABValue == 1)
     SetupWindow("A<>B,A is 1");
   else
     SetupWindow("A<>B,B is 1");

   GUI_LoadPic1(8,15,(uint8_t *)ABIco,42,16);
   
   char cacheBuf[10];
   sprintf(cacheBuf,"%d",ABSetIndex);    
   GUI_PutString(10,40,cacheBuf);
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(ParaSetConfirm == 2)
      {
        ParaSetConfirm = 1;
        ModbusMasterAsk(SlaverAddr,40034,0x06,0x00,ABSetIndex);
        ABValue = ABSetIndex;
        FlashRsvWrite(&ABValue,0,1);
        rt_timer_init(&soft_timer5, "ParameterInqury",       
                          ParameterInqury,                          
                          RT_NULL,                                   
                          500,                                          
                          RT_TIMER_FLAG_ONE_SHOT);                     
                
        rt_timer_start(&soft_timer5);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(ParaSetConfirm == 2)     //????????
      {
        ParaSetConfirm = 0;
        ParaMenuFlag = 0;      
      }
   }
}

void ParameterSetupMenu(uint8_t ctr)
{
  switch(ctr)
  {
    case 0x00:
      ForceMenuShow();
      break;
    case 0x01:
      InoutMenuShow();
      break;
    case 0x02:
      SpacingMenuShow();
      break;
    case 0x03:
      lengthMenuShow();
      break;
    case 0x04:
      MovementMenuShow();
      break;
    case 0x05:
      IndelayMenuShow();
      break;
    case 0x06:
      OutnumMenuShow();
      break;
    case 0x07:
      ABMenuShow();
      break;
    case 0x08:
      ParaMenuFlag = 0; 
      ParaSetConfirm = 0;
      GUI_WindowsHide(&winxp);
      PowerOnFlag = 0;
      SetBit(PowerOnFlag,Menupage);       //?????????????????
      break;
  }
}

void ParameterInqury(void* parameter)
{
  BatFlag = 2;       //?????
  EnableRead = 1;
 
  switch(ParaChooseIndex)
  {
    case 0x00:
      ModbusMasterRead(SlaverAddr,40002,0x03,0x00,0x01);
      break;
    case 0x01:
      ModbusMasterRead(SlaverAddr,40003,0x03,0x00,0x01);
      break;
    case 0x02:
      ModbusMasterRead(SlaverAddr,40004,0x03,0x00,0x01);
      break;
    case 0x03:
      ModbusMasterRead(SlaverAddr,40005,0x03,0x00,0x01);
      break;
    case 0x04:
      ModbusMasterRead(SlaverAddr,40006,0x03,0x00,0x01);
      break;
    case 0x05:
      ModbusMasterRead(SlaverAddr,40007,0x03,0x00,0x01);
      break;
    case 0x06:
      ModbusMasterRead(SlaverAddr,40030,0x03,0x00,0x01);
      break;
    case 0x07:
      ModbusMasterRead(SlaverAddr,40034,0x03,0x00,0x01);
      break;
  }
}

static void SelectFrebandMenuShow(void)
{
   SetupWindow("Current Band");
   GUI_LoadPic1(8,15,(uint8_t *)NowFrequeceIco,56,16);
   
   switch(FreSetIndex)         //????????
   {
    case 1:
      GUI_PutString(8,45,"433.920Mhz");
      break;
    case 2:
      GUI_PutString(8,45,"432.920Mhz");
      break;
    case 3:
      GUI_PutString(8,45,"431.920Mhz");
      break;
    case 4:
      GUI_PutString(8,45,"434.920Mhz");
      break;
    case 5:
      GUI_PutString(8,45,"435.920Mhz");
      break;
    case 6:
      GUI_PutString(8,45,"436.920Mhz");
      break;
    case 7:
      GUI_PutString(8,45,"437.920Mhz");
      break;
    case 8:
      GUI_PutString(8,45,"438.920Mhz");
      break;
   }
   
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(SystemSetConfirm == 2)
      {
        SystemSetConfirm = 1;
        
        switch(FreSetIndex)         //"ok" ???,??????
        {
          case 1:
            SetFrequency(0x00,433920);
            break;
          case 2:
            SetFrequency(0x00,432920);
            break;
          case 3:
            SetFrequency(0x00,431920);
            break;
          case 4:
            SetFrequency(0x00,434920);
            break;
          case 5:
            SetFrequency(0x00,435920);
            break;
          case 6:
            SetFrequency(0x00,436920);
            break;
          case 7:
            SetFrequency(0x00,437920);
            break;
          case 8:
            SetFrequency(0x00,438920);
            break;
        }
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(SystemSetConfirm == 2)     //????????
      {
        SystemSetConfirm = 0;
        SystemSetFlag = 0; 
      }
   }
   

}


/*******************************************************************************
* ???    : SysSetFunction
* ??	    : ????????
* ????  : ctr,KeyIndex
* ????  : ?
*******************************************************************************/
void SysSetFunction(uint8_t ctr,uint8_t KeyIndex)
{
    switch(ctr)
    {
      case 0:               //??????
        FreSetIndex = KeyIndex;
        if(FreSetIndex > 8)
          FreSetIndex = 1;
        break;
      case 3:               //????????
        TurnoffIndex = KeyIndex;
        
        if(TurnoffIndex > 9)
          TurnoffIndex = 1; 
        break;
    }
}

static void BackLightSetup(void)
{
  SetupWindow("BackLight Setup");
  
  GUI_PutString(15,35,"Wait For Develop!");
}

static void BackLightTime(void)
{
  SetupWindow("BackLightTime(1-5)");  
  GUI_PutString(15,35,"Wait For Develop!");
}

static void RegisterID(void)
{
  char cacheBuf[10];
  static long CpuID = 0;
  CpuID = Lock_Code%100000;           //??ID????40040
  ModbusMasterAsk(SlaverAddr,40040,0x06,0x00,CpuID);
  
  SetupWindow("Register ID");  
  GUI_PutString(25,35,"CPU ID :");
  
  sprintf(cacheBuf,"%d",CpuID);    
  GUI_PutString(75,35,cacheBuf);
}

void DelayTurnoff(void* parameter)
{
  TurnoffTick ++;
    
  if(TurnoffTick == TurnoffValue)
  {
    rt_timer_stop(&soft_timer3);
    CtrPowerOff();
  }
}

static void PoweroffTime(void)
{
   SetupWindow("Power off(1-9)");  
   
   switch(TurnoffIndex)                  //??
   {
     case 1:
       GUI_LoadPic1(18,35,(uint8_t *)AlwaysOnIco,28,16);
       break;
     case 2:
       GUI_PutString(30,20,"5");          //5?????
       TurnoffValue = 5;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco1,64,16);
       break;
     case 3:
       GUI_PutString(30,20,"10");         //10?????
       TurnoffValue = 10;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco1,64,16);
       break;
     case 4:
       GUI_PutString(30,20,"20");         //20?????
       TurnoffValue = 20;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco1,64,16);
       break;
     case 5:
       GUI_PutString(30,20,"30");         //30?????
       TurnoffValue = 30;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco1,64,16);
       break;
     case 6:
       GUI_PutString(30,20,"1");          //1?????     
       TurnoffValue = 60;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco,64,16);
       break;
     case 7:
       GUI_PutString(30,20,"5");          //5?????     
       TurnoffValue = 300;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco,64,16);
       break;
     case 8:
       GUI_PutString(30,20,"10");         //10?????     
       TurnoffValue = 600;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco,64,16);
       break;
     case 9:
       GUI_PutString(30,20,"20");         //20?????     
       TurnoffValue = 1200;
       GUI_LoadPic1(4,35,(uint8_t *)PowerOffTimeIco,64,16);
       break;
   }
  
   if(OKorCancel == 1)
   {
      GUI_Button_OK1(70,20);
      GUI_Button_Cancle(70,40);
      
      if(SystemSetConfirm == 2)
      {
        SystemSetConfirm = 1;
        FlashRsvWrite(&TurnoffIndex,1,1);

        rt_timer_start(&soft_timer3);
      }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(SystemSetConfirm == 2)     //????????
      {
        SystemSetConfirm = 0;
        SystemSetFlag = 0;      
      }
   }
}


void SystemSetupMenu(int8_t ctr)
{
  switch(ctr)
  {
    case 0x00:
      SelectFrebandMenuShow();
      break;
    case 0x01:
      BackLightSetup();
      break;
    case 0x02:
      BackLightTime();
      break;
    case 0x03:
      PoweroffTime();
      break;
    case 0x04:
      RegisterID();
      break;
    case 0x05:
      SystemSetFlag = 0;
      PowerOnFlag = 0;
      SetBit(PowerOnFlag,Menupage);
      break;
  }
}

void SelectReceiverMenuShow(void)
{
   SetupWindow("Connect Receiver");
   GUI_LoadPic1(8,15,(uint8_t *)ChooseDevIco,56,16);
   
   if(DevIdChooseIndex > 8)
     DevIdChooseIndex = 1;
   
   switch(DevIdChooseIndex)
   {
      case 0x01:
        GUI_PutString(10,40,"1#");
        break;
      case 0x02:
        GUI_PutString(10,40,"2#");
        break;
      case 0x03:
        GUI_PutString(10,40,"3#");
        break;
      case 0x04:
        GUI_PutString(10,40,"4#");
        break;
      case 0x05:
        GUI_PutString(10,40,"5#");
        break;
      case 0x06:
        GUI_PutString(10,40,"6#");
        break;
      case 0x07:
        GUI_PutString(10,40,"7#");
        break;
      case 0x08:
        GUI_PutString(10,40,"8#");
        break;
   }
   
   if(OKorCancel == 1)
   {
     GUI_Button_OK1(70,20);
     GUI_Button_Cancle(70,40);
     
     if(DevIDConfirm == 1)
     {
      DevIDConfirm = 0;
      SetSlaverAddr(0x0B,DevIdChooseIndex);
     }
   }
   else
   {
      GUI_Button_OK(70,20);
      GUI_Button_Cancle1(70,40);
      
      if(DevIDConfirm == 1)     //????????
      {
        DevIDConfirm = 0;
        PowerOnFlag = 0; 
        SetBit(PowerOnFlag,Menupage);
      }
   }
}




/*******************************************************************************
* ???    : CipherCheckFunction
* ??	    : ??????
* ????  : KeyIndex
* ????  : ?
********************************************************************************
*??     ??            ??            ??
*V0.1    Guolz         2017/12/21       ????
*******************************************************************************/
void CipherCheckFunction(uint8_t KeyIndex)
{
    CodeSum *= 10;
    CodeSum += KeyIndex;
    
    TypeCodeTick ++;
    if(TypeCodeTick == 3)
    {
      TypeCodeTick = 0;
      
      if(CodeSum != CodeValue)     //??????
        CodeSum = 0;
    }
    
    if(CodeSum == CodeValue)
    {
      CodeSum = 0;
      CipherLineNum = 95;
      TypeCodeTick = 0; 
      ResetBit(Menuhierarchy,0);
      //??????
      ResetBit(PowerOnFlag,0);
      SetBit(PowerOnFlag,1);    //????????
    }
}

/*******************************************************************************
* ???    : ParaSetFunction
* ??	    : ??????
* ????  : ctr,KeyIndex
* ????  : ?
*******************************************************************************/
void ParaSetFunction(uint8_t ctr,uint8_t KeyIndex)
{
    switch(ctr)
    {
      case 0:               //???? 0?1
        ForceSetIndex = KeyIndex;
        if(ForceSetIndex > 1)
          ForceSetIndex = 0;
        break;
      case 1:               //???? 0?1
        InOutIndex = KeyIndex;
        if(InOutIndex > 1)
          InOutIndex = 0;
        break;
      case 2:               //????
        SpacingIndex *= 10;         
        SpacingIndex += KeyIndex;
        
        if(SpacingIndex > 500)
          SpacingIndex = 0;  
        break; 
      case 3:               //????       
        LengthSetIndex *= 10;         
        LengthSetIndex += KeyIndex;
        
        if(LengthSetIndex > 2000)
          LengthSetIndex = 0;
        break;
      case 4:              //?????                                       
        MovementSetIndex *= 10;         
        MovementSetIndex += KeyIndex;
        
        if(MovementSetIndex > 200)
          MovementSetIndex = 0;
        break;
      case 5:              //?????                             
        DelaySetIndex *= 10;         
        DelaySetIndex += KeyIndex;
        
        if(DelaySetIndex > 200)
          DelaySetIndex = 0;
        break;
      case 6:              //???                         
        OutNumIndex *= 10;
        OutNumIndex += KeyIndex; 
        
        if(OutNumIndex > 200)
          OutNumIndex = 0;
        break;
      case 7:              //AB??  0?1
        ABSetIndex = KeyIndex;
        if(ABSetIndex > 1)
          ABSetIndex = 0;      
        break;
    }
}

void PoweroffMenu(void)
{
  SetupWindow("Turn off after 2s");
  GUI_LoadPic1(30,25,(uint8_t *)PoweroffIco,64,32);
}


void LanguageSetMenu(void)
{
   SetupWindow("Language");
   
   if(LanguageIndex == 0)
   {
     GUI_LoadPic(28,25,(uint8_t *)ChineseIco,28,16);
     GUI_LoadPic1(70,25,(uint8_t *)EnglishIco,28,16);
   }
   else
   {
     GUI_LoadPic1(28,25,(uint8_t *)ChineseIco,28,16);
     GUI_LoadPic(70,25,(uint8_t *)EnglishIco,28,16);
   }
}



/* ??thread_Menu_show_entry????? */
void thread_Menu_show_entry(void* parameter){
    rt_thread_delay(10);
    while (1){
        if(GetBit(PowerOnFlag,Homepage) == 1){
            HomePageShow();                             //主页
        }else if(GetBit(PowerOnFlag,Menupage) == 1){
            SetupPageShow(MenuChooseIndex);             //设置菜单
        }else if(GetBit(PowerOnFlag,Recvpage) == 1){
            SelectReceiverMenuShow();                   //选择机号界面
        }else if(GetBit(PowerOnFlag,Parapage) == 1){
            if(ParaMenuFlag == 0)
                ParameterSetShow(ParaChooseIndex);      //参数设置界面
            else{
                if(ParaAskFlag == 1){
                    ParaAskFlag = 0;
                    ParameterInqury(NULL);
                }
                ParameterSetupMenu(ParaChooseIndex);    //进入参数设置子菜单页面
            }
        }else if(GetBit(PowerOnFlag,Langpage) == 1){
          LanguageConfirm = 1;
          LanguageSetMenu();
        }else if(GetBit(PowerOnFlag,Systpage) == 1){
            if(SystemSetFlag == 0)                      //系统设置界面
                SystemSetPageShow(SysChooseIndex);
            else
                SystemSetupMenu(SysChooseIndex);        //进入系统设置子菜单页面
        }else if(GetBit(PowerOnFlag,Exitpage) == 1){
            PowerOnFlag = 0;
            GUI_WindowsHide(&winxp);
            SetBit(PowerOnFlag,Homepage);
        }else if(GetBit(PowerOnFlag,Poffpage) == 1){
            PoweroffMenu();
        }

        GUI_Exec();            //刷新屏幕
        rt_thread_delay(25);
        AdcSystick100Routine();
    }
}





