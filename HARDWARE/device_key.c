/*
********************************************************************************
*                            中国自主物联网操作系统
*                            Thread Operating System
*
*                              主芯片:STM32F401re
*
********************************************************************************
*文件名     : device_key.c
*作用       : 按键设备
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2016/12/14        初始版本
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "device_key.h"

struct rt_timer power_key_det_timer;
struct rt_timer soft_timer1;
struct rt_timer soft_timer2;
struct rt_timer soft_timer3;
struct rt_timer soft_timer4;

extern WINDOWS HomePage;

static uint8_t KeyValue = 0;

uint8_t CMD1[8] = {0x0C,0x06,0x00,0x00,0x02,0x80,0x88,0x17};
uint8_t CMD2[8] = {0x0C,0x06,0x00,0x00,0x00,0x80,0x89,0x77};
uint8_t CMD3[8] = {0x0C,0x06,0x00,0x00,0x03,0x00,0x88,0x27};
uint8_t CMD4[8] = {0x0C,0x06,0x00,0x00,0x01,0x00,0x89,0x47};
uint8_t CMD5[8] = {0x0C,0x06,0x00,0x00,0x00,0x00,0x88,0xD7};



uint8_t CheckCode = 0;
uint8_t CodeLineNum = 0;
uint8_t KeyIndex = 0;       //?????????


/** @defgroup STM32F4XX_LOW_LEVEL_Private_Variables STM32F4XX LOW LEVEL Private Variables
  * @{
  */ 
    
GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY_BUTTON_PIN}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};

uint8_t ChangeCMD[15] = {0xff,0x56,0xae,0x35,0xa9,0x55,0x90,0x06,0x9f,0x00,0x04,0x06,0x05,0x00,0x00};



/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  */
void Key_init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable the BUTTON Clock */
    BUTTONx_GPIO_CLK_ENABLE(Button);
    FunctionPort2Enable;
    FunctionPort3Enable;
    
    if(ButtonMode == BUTTON_MODE_GPIO)
    {
      /* Configure Button pin as input */
      GPIO_InitStruct.Pin = BUTTON_PIN[Button];
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
      
      GPIO_InitStruct.Pin = FunctionPin1;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      HAL_GPIO_Init(FunctionPort1, &GPIO_InitStruct);
      
      GPIO_InitStruct.Pin = FunctionPin2;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      HAL_GPIO_Init(FunctionPort2, &GPIO_InitStruct);
      
      GPIO_InitStruct.Pin = FunctionPin3;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      HAL_GPIO_Init(FunctionPort3, &GPIO_InitStruct);
      
      GPIO_InitStruct.Pin = ChargingPin;
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
      HAL_GPIO_Init(FunctionPort3, &GPIO_InitStruct);
    }
    
    if(ButtonMode == BUTTON_MODE_EXTI)
    {
      /* Configure Button pin as input with External interrupt */
      GPIO_InitStruct.Pin = BUTTON_PIN[Button];
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; 
      HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
      
      /* Enable and set Button EXTI Interrupt to the lowest priority */
      HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
    }
}


/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY  
  * @retval The Button GPIO pin value.
  */
uint32_t GetKeyState(Button_TypeDef Button)
{
    return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}



static void SingleClickDetect(void* parameter)
{
    if(GetKeyState(BUTTON_KEY) == GPIO_PIN_SET)
    {
        if(GetBit(PowerOnFlag,Homepage) == 1)      //主页
        {
          if(GetBit(Menuhierarchy,0) == 1)         //输入密码
          {
            CodeSum = 0;   
            TypeCodeTick = 0;                      //清空密码输入区，主页正常业务逻辑解析显示
            ResetBit(Menuhierarchy,0);
            HomePage.title = "HOME";
          }
          else
            SetBit(Menuhierarchy,0);               //输入密码
        } 
        else if(GetBit(PowerOnFlag,Menupage) == 1)//设置菜单
        {
          PowerOnFlag = 0;
          ResetBit(Menuhierarchy,0);  
          SetBit(PowerOnFlag,Homepage);            //返回主页
          CodeSum = 0;                             //清空密码
          TypeCodeTick = 0; 
        }
        else if((GetBit(PowerOnFlag,Recvpage) == 1)||(GetBit(PowerOnFlag,Langpage) == 1))
        {
          GUI_WindowsHide(&winxp);
          PowerOnFlag = 0;
          SetBit(PowerOnFlag,Menupage);            //从各个子菜单进入设置菜单页面
        }
        else if(GetBit(PowerOnFlag,Parapage) == 1)
        {
          switch(ParaMenuFlag)
          {
            case 0x00:
              GUI_WindowsHide(&winxp);
              PowerOnFlag = 0;
              SetBit(PowerOnFlag,Menupage);       //从各个参数设置菜单返回设置菜单页面
              break;
            case 0x01:
              ParaSetConfirm = 0;
              ParaMenuFlag = 0;                   //从各个参数设置子菜单返回参数菜单页面
              break;
          }
        }
        else if(GetBit(PowerOnFlag,Systpage) == 1)
        {
          switch(SystemSetFlag)
          {
            case 0x00:
              GUI_WindowsHide(&winxp);
              PowerOnFlag = 0;
              SetBit(PowerOnFlag,Menupage);       //从各个参数设置菜单返回设置菜单页面
              break;
            case 0x01:
              SystemSetFlag = 0;
              SystemSetConfirm = 0;              //从各个参数设置子菜单返回系统设置页面
              break;
          }
        }
    }
}


void EnableKeyDetect(void* parameter)
{
   ResetBit(Menuhierarchy,1);
}

static void PowerDownDevice(void* parameter)
{
   CtrPowerOff();
}


void ShowString(uint8_t *data,uint8_t *sh,uint8_t len)
{
    uint8_t m =0;
    uint8_t dat;
    
    for(m=0;m<len;m++)
    {
        dat = (data[m]&0xf0)>>4;
        sh[2*m] = HexToAscii(dat);
        dat = data[m]&0xf;
        sh[2*m+1] = HexToAscii(dat);
    }
}

/*******************************************************************************
* 函数名    : KeyProcess
* 描述	    : 按键消息处理函数
* 输入参数  : keyInfo
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/12/21       初始版本
*******************************************************************************/
void KeyProcess(uint8_t keyInfo)
{
    switch(keyInfo)
    {
      case Key1Press:
        if(GetBit(Menuhierarchy,0) == 1)       //密码
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x02);
        break;
      case Key1Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key2Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }       
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x80);
        break;
      case Key2Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key3Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x04);
        break;
      case Key3Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key4Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x04,0x00);
        break;
      case Key4Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key5Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x40);
        break;
      case Key5Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key6Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x08,0x00);
        break;
      case Key6Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key7Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x01);
        break;
      case Key7Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key8Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x01,0x00);
        break;
      case Key8Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key9Press:
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))    //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Systpage)&&(SystemSetFlag == 1))   //系统设置
        {
          SysSetFunction(SysChooseIndex,KeyValue);
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)     //选择机号
          DevIdChooseIndex = KeyValue;
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x20);
        break;
      case Key9Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key11Press:    //“0”键
        if(GetBit(Menuhierarchy,0) == 1)
          CipherCheckFunction(KeyValue-11);
        if(GetBit(PowerOnFlag,Parapage)&&(ParaMenuFlag == 1))   //参数设置
        {
          ParaSetFunction(ParaChooseIndex,KeyValue-11);
        }
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x08);
        break;
      case Key11Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key12Press:                          //"上"
        if(GetBit(PowerOnFlag,Menupage) == 1)   //菜单设置界面
        {
          MenuChooseIndex --;
          if(MenuChooseIndex == -1)
            MenuChooseIndex = 4;
        }
        if(GetBit(PowerOnFlag,Parapage) == 1)   //参数选择界面
        {
          if(ParaMenuFlag == 0)
          {
            ParaChooseIndex --;
            if(ParaChooseIndex == -1)
              ParaChooseIndex = 8;
          }
          else
          {
            OKorCancel --;
            if(OKorCancel == -1)
              OKorCancel = 1;
          }
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)   //接收机选择界面
        {
            OKorCancel --;
            if(OKorCancel == -1)
              OKorCancel = 1;
        }
        if(GetBit(PowerOnFlag,Systpage) == 1)  //系统设置界面
        {
          if(SystemSetFlag == 0)
          {
            SysChooseIndex --;
            if(SysChooseIndex == -1)
              SysChooseIndex = 5;
          }
          else
          {
            OKorCancel --;
            if(OKorCancel == -1)
              OKorCancel = 1;
          }
        }
        if(GetBit(PowerOnFlag,Langpage) == 1)
        {
          LanguageIndex--;
          if(LanguageIndex == -1)
            LanguageIndex = 1;
        }
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x10,0x00);
        break;
      case Key12Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key13Press:                          //"确认"
        switch(PowerOnFlag)
        {
          case 0x02:                            //菜单选择界面，点击确认
            ResetBit(PowerOnFlag,Menupage);
            GUI_WindowsHide(&winxp);
            SetBit(PowerOnFlag,MenuChooseIndex+2);
          break;
          case 0x04:                            //机号选择界面，点击确认
            DevIDConfirm ++;
            break;
          case 0x08:     //参数选择界面
            ParaMenuFlag = 1;
            if(ParaSetConfirm == 0)
              ParaAskFlag = 1;          //用于问询寄存器值
            ParaSetConfirm ++;
          break;
          case 0x20:    //系统设置界面
            SystemSetFlag = 1;
            SystemSetConfirm ++;
          break;
        }
        if((GetBit(PowerOnFlag,Langpage) == 1)&&(LanguageConfirm == 1))
        {
          LanguageConfirm = 0;
          PowerOnFlag = 0;
          SetBit(PowerOnFlag,Menupage);
        }
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x20,0x00);
        break;
      case Key13Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
      case Key14Press:                          //"下"
        if(GetBit(PowerOnFlag,Menupage) == 1)   //菜单设置界面
        {
          MenuChooseIndex ++;
          if(MenuChooseIndex == 5)
            MenuChooseIndex = 0;
        }
        if(GetBit(PowerOnFlag,Parapage) == 1)   //参数选择界面
        {
          if(ParaMenuFlag == 0)
          {
            ParaChooseIndex ++;
            if(ParaChooseIndex == 9)
              ParaChooseIndex = 0;
          }
          else
          {
            OKorCancel ++;
            if(OKorCancel == 2)
              OKorCancel = 0;
          }
        }
        if(GetBit(PowerOnFlag,Recvpage) == 1)   //参数选择界面
        {
            OKorCancel ++;
            if(OKorCancel == 2)
              OKorCancel = 0;
        }
        if(GetBit(PowerOnFlag,Systpage) == 1)  //系统设置界面
        {
          if(SystemSetFlag == 0)
          {
            SysChooseIndex ++;
            if(SysChooseIndex == 6)
              SysChooseIndex = 0;
          }
          else
          {
            OKorCancel ++;
            if(OKorCancel == 2)
              OKorCancel = 0;
          }
        }
        if(GetBit(PowerOnFlag,Langpage) == 1)
        {
          LanguageIndex++;
          if(LanguageIndex == 2)
            LanguageIndex = 0;
        }
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x10);
        break;
      case Key14Release:
        if((GetBit(Menuhierarchy,0) == 0)&&(GetBit(PowerOnFlag,Homepage) == 1))     //主页解析
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x00);        //松开键
        break;
    }
}


//BEE键长按功能键
static void KeyBeeFunction(void *para)
{
  static uint8_t KeyBeeTick = 0;
  
  KeyBeeTick ++;
  if(KeyBeeTick == 4)
  {
    KeyBeeTick = 0;
    EnableRead = 0;
    ModbusMasterAsk(SlaverAddr,40001,0x06,0x40,0x00);
    EnableRead = 1;
    HomePage.title = (uint8_t *)"BeeLongPress";
  }
  
  if(HAL_GPIO_ReadPin(FunctionPort2,GPIO_PIN_11) == GPIO_PIN_SET){
        rt_timer_start(&soft_timer4);
	  }
  else
  {
    KeyBeeTick = 0;
    rt_timer_delete(&soft_timer4);
  }
}
    
static uint8_t multiKey = 0;

/*******************************************************************************
* 函数名    : KeyScan_Result
* 描述	    : 例行按键扫描函数，
* 输入参数  : 无
* 返回参数  : 键值
********************************************************************************
*版本     作者            日期            说明
*V0.1    Guolz         2017/12/21       初始版本
*******************************************************************************/
uint8_t KeyScan_Result(void) 
{
    uint8_t key_val = 0;
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_10) == GPIO_PIN_SET)
    {
        if(KeyValue != 1)
        {
          KeyValue = 1;
          key_val = Key1Press;
          HomePage.title = (uint8_t *)"Key1Press";
        }
 //       __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
		
    }
    else
    {
        if(KeyValue == 1)
        {
          KeyValue = 0;
          key_val = Key1Release;
                      
          HomePage.title = (uint8_t *)"Key1Release";
        }
		
    }
	
	
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_7) == GPIO_PIN_SET)
    {
        if((KeyValue != 2)&&(multiKey == 0))
        {
          KeyValue = 2;
          HomePage.title = (uint8_t *)"Key2Press";
          key_val = Key2Press;
        }
    } 
    else
    {
        if(KeyValue == 2)
        {
          KeyValue = 0;
          key_val = Key2Release;
          HomePage.title = (uint8_t *)"Key2Release";
          if(multiKey == 2)
            multiKey = 0;
        }
        
        if(multiKey == 1)
        {
            KeyValue = 4;
            multiKey = 0;
            key_val = Key2Release;
            HomePage.title = (uint8_t *)"Key2looseKey4Press";
        }
    }
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_13) == GPIO_PIN_SET)
    {
        if(KeyValue != 3)
        {
          KeyValue = 3;
          key_val = Key3Press;
          HomePage.title = (uint8_t *)"Key3Press";
        }
    } 
    else
    {
        if(KeyValue == 3)
        {
          KeyValue = 0;
          key_val = Key3Release;
          HomePage.title = (uint8_t *)"Key3Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_2) == GPIO_PIN_SET)
    {
        if((KeyValue == 2)&&(multiKey == 0))
        {
//          System.Device.Usart1.WriteNData(CMD1,8);
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x02,0x80);
          multiKey = 1;      //组合键标志数值
          KeyValue = 4;
          HomePage.title = (uint8_t *)"Key2-4Press";
        }
        else if((KeyValue == 8)&&(multiKey == 0))
        {
 //         System.Device.Usart1.WriteNData(CMD3,8);
          ModbusMasterAsk(SlaverAddr,40001,0x06,0x03,0x00);
          multiKey = 3;      //组合键标志数值
          KeyValue = 4;
          HomePage.title = (uint8_t *)"Key8-4Press";
        }
        if(KeyValue != 4)
        {
          KeyValue = 4;
          key_val = Key4Press;
          HomePage.title = (uint8_t *)"Key4Press";
        }
    } 
    else
    {
        if(KeyValue == 4)
        {
          if(multiKey == 1)
          {
 //           System.Device.Usart1.WriteNData(CMD2,8);
            ModbusMasterAsk(SlaverAddr,40001,0x06,0x00,0x80);
            
            if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_7) == GPIO_PIN_SET)
            {
              KeyValue = 2;
              multiKey = 0;
              HomePage.title = (uint8_t *)"Key2Press";
            }
            else
            {
              KeyValue = 0;
              multiKey = 0;
            }
          }
          else if(multiKey == 3)
          {
 //           System.Device.Usart1.WriteNData(CMD4,8);
            ModbusMasterAsk(SlaverAddr,40001,0x06,0x01,0x00);
            
            if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_5) == GPIO_PIN_SET)
            {
              KeyValue = 8;
              multiKey = 0;
              HomePage.title = (uint8_t *)"Key8Press";
            }
            else
            {
              KeyValue = 0;
              multiKey = 0;
            }
          }
          else if(multiKey == 0)
          {
            KeyValue = 0;
            key_val = Key4Release;
            HomePage.title = (uint8_t *)"Key4Release";
          }
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_6) == GPIO_PIN_SET)
    {
        if(KeyValue != 5)
        {
          KeyValue = 5;
          key_val = Key5Press;
          HomePage.title = (uint8_t *)"Key5Press";
        }
    } 
    else
    {
        if(KeyValue == 5)
        {
          KeyValue = 0;
          key_val = Key5Release;
          HomePage.title = (uint8_t *)"Key5Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort3,GPIO_PIN_15) == GPIO_PIN_SET)
    {
        if(KeyValue != 6)
        {
          KeyValue = 6;
          key_val = Key6Press;
          HomePage.title = (uint8_t *)"Key6Press";
        }
    }
    else
    {
        if(KeyValue == 6)
        {
          KeyValue = 0;
          key_val = Key6Release;
          HomePage.title = (uint8_t *)"Key6Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_1) == GPIO_PIN_SET)
    {
        if(KeyValue != 7)
        {
          KeyValue = 7;
          key_val = Key7Press;
          HomePage.title = (uint8_t *)"Key7Press";
        }
    } 
    else
    {
        if(KeyValue == 7)
        {
          KeyValue = 0;
          key_val = Key7Release;
          HomePage.title = (uint8_t *)"Key7Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_5) == GPIO_PIN_SET)
    {
        if((KeyValue != 8)&&(multiKey == 0))
        {
          KeyValue = 8;
          key_val = Key8Press;
          HomePage.title = (uint8_t *)"Key8Press";
        }
    }
    else
    {
        if(KeyValue == 8)
        {
          KeyValue = 0;
          key_val = Key8Release;
          System.Device.Usart1.WriteNData(CMD5,8);
          
          if(multiKey == 4)
            multiKey = 0;
          
          HomePage.title = (uint8_t *)"Key8Release";
        }
        
        if(multiKey == 3)
        {
            KeyValue = 4;
            multiKey = 0;
            HomePage.title = (uint8_t *)"Key8looseKey4Press";
            key_val = Key8Release;
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort2,GPIO_PIN_10) == GPIO_PIN_SET)
    {
        if(KeyValue != 9)
        {
          KeyValue = 9;
          key_val = Key9Press;
          HomePage.title = (uint8_t *)"Key9Press";
        }
    } 
    else
    {
        if(KeyValue == 9)
        {
          KeyValue = 0;
          key_val = Key9Release;
          HomePage.title = (uint8_t *)"Key9Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_4) == GPIO_PIN_SET)
    {
        if(KeyValue != 11)
        {
          KeyValue = 11;
          key_val = Key11Press;
          HomePage.title = (uint8_t *)"Key11Press";
        }
    }
    else
    {
        if(KeyValue == 11)
        {
          KeyValue = 0;
          key_val = Key11Release;
          HomePage.title = (uint8_t *)"Key11Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort2,GPIO_PIN_11) == GPIO_PIN_SET)           //"?"?
    {
        if(KeyValue != 12)
        {
          KeyValue = 12;
          key_val = Key12Press;
          HomePage.title = (uint8_t *)"Key12Press";
          rt_timer_init(&soft_timer4, "KeyBeeFunction",        
                          KeyBeeFunction,                          
                          RT_NULL,                                     
                          1000,                                         
                          RT_TIMER_FLAG_ONE_SHOT);                     
                
          rt_timer_start(&soft_timer4);
        }
    } 
    else
    {
        if(KeyValue == 12)
        {
          KeyValue = 0;
          key_val = Key12Release;
          HomePage.title = (uint8_t *)"Key12Release";
          rt_timer_delete(&soft_timer4);
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort1,GPIO_PIN_0) == GPIO_PIN_SET)            //"??"?
    {
        if(KeyValue != 13)
        {
          KeyValue = 13;
          key_val = Key13Press;
          HomePage.title = (uint8_t *)"Key13Press";
        }
    } 
    else
    {
        if(KeyValue == 13)
        {
          KeyValue = 0;
          key_val = Key13Release;
          HomePage.title = (uint8_t *)"Key13Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort2,GPIO_PIN_12) == GPIO_PIN_SET)            //"?"?
    {
        if(KeyValue != 14)
        {
          KeyValue = 14;
          key_val = Key14Press;
          HomePage.title = (uint8_t *)"Key14Press";
        }
    } 
    else
    {
        if(KeyValue == 14)
        {
          KeyValue = 0;
          key_val = Key14Release;
          HomePage.title = (uint8_t *)"Key14Release";
        }
    }
    
    if(HAL_GPIO_ReadPin(FunctionPort3,GPIO_PIN_1) == GPIO_PIN_RESET)   
      ChargingFlag = 1;
    else
      ChargingFlag = 0;
 
    
    static uint8_t ReStart = 0;
    if((KeyValue != 0)||(TurnoffIndex == 1))
    {      
      ReStart = 1;
      TurnoffTick = 0;
      rt_timer_stop(&soft_timer3);            //有按键操作，停止延时关机计时
    }
    else
    {
      if(ReStart == 1)                        //无按键操作，开始延时关机计时
      {
        ReStart = 0;
        rt_timer_init(&soft_timer3, "DelayTurnoff",        
          DelayTurnoff,                                     
          RT_NULL,                                      // 超时函数的入口参数 
          1000,                                          
          RT_TIMER_FLAG_PERIODIC);                     
        
        rt_timer_start(&soft_timer3);
      }
    }

    return key_val;
}

static void KeyStatusDetect_Callback(void* parameter)
{
    static uint8_t KeySysTick = 0;
    static uint8_t CmdTick = 0;

    if(GetBit(Menuhierarchy,1) == 0){      //开机后，缓1s再进行扫描
        if(GetKeyState(BUTTON_KEY) == GPIO_PIN_RESET)
        {
            KeySysTick ++;

            if(KeySysTick == 1){
                rt_timer_init(&soft_timer1, "SingleClickDetect",        // 定时器名字是 SingleClickDetect 
                          SingleClickDetect,                            // 超时时回调的处理函数 
                          RT_NULL,                                      // 超时函数的入口参数 
                          200,                                          // 定时长度，以OS Tick为单位，即20个OS Tick
                          RT_TIMER_FLAG_ONE_SHOT);                      // 单次
                
                rt_timer_start(&soft_timer1);
            }
            
            if(KeySysTick == 100)
            {
                KeySysTick = 0;
                CmdTick++;
                
                if(CmdTick == 1)
                {
                    CtrPowerOn();
                    HomePage.title = (uint8_t *)"HOME";
                    SetBit(Menuhierarchy,1);
                    SetBit(PowerOnFlag,0);

                    rt_timer_init(&soft_timer1, "EnableKeyDetect",        // 定时器名字是 EnableKeyDetect 
                    EnableKeyDetect,                                      // 超时时回调的处理函数 
                    RT_NULL,                                              // 超时函数的入口参数 
                    1000,                                                 // 定时长度，以OS Tick为单位
                    RT_TIMER_FLAG_ONE_SHOT);                              // 单次

                    rt_timer_start(&soft_timer1);
                }
                else if(CmdTick == 2)
                {
                    CmdTick = 0;
                    PowerOnFlag = 0;
                    SetBit(PowerOnFlag,Poffpage);
                    rt_timer_init(&soft_timer2, "PowerDownDevice",        // 定时器名字是 PowerDownDevice 
                    PowerDownDevice,                                      // 超时时回调的处理函数 
                    RT_NULL,                                              // 超时函数的入口参数 
                    1000,                                                 // 定时长度，以OS Tick为单位，
                    RT_TIMER_FLAG_ONE_SHOT);                              // 单次

                    rt_timer_start(&soft_timer2);
                }
            }
        }
        else
          KeySysTick = 0;
    }      
}

static void CheckTurnPower(void)
{
    if((TurnoffIndex > 1)&&(TurnoffIndex < 10))
    {
       switch(TurnoffIndex)
       {
         case 2:
           TurnoffValue = 5;
           break;
         case 3:
           TurnoffValue = 10;
           break;
         case 4:
           TurnoffValue = 20;
           break;
         case 5:
           TurnoffValue = 30;
           break;
         case 6:
           TurnoffValue = 60;
           break;
         case 7:
           TurnoffValue = 300;
           break;
         case 8:
           TurnoffValue = 600;
           break;
         case 9:
           TurnoffValue = 1200;
           break;
       }
       
       rt_timer_init(&soft_timer3, "DelayTurnoff",        
          DelayTurnoff,                                     
          RT_NULL,                                      // 超时函数的入口参数 
          1000,                                          
          RT_TIMER_FLAG_PERIODIC);                     
        
        rt_timer_start(&soft_timer3);
    }
}



void thread_Key_scan_entry(void* parameter){
    rt_thread_delay(20);
    static uint8_t last_key_val = 0;
    static uint8_t current_key_val = 0;
    static uint8_t AppTick = 0;
    static uint8_t FailTick = 0;
    static uint8_t BatTick = 0;
    uint16_t regAddr = 0;

    rt_timer_init(&power_key_det_timer, "power_key_det_timer",              // 定时器名字是 power_key_det_timer
              KeyStatusDetect_Callback,                                     // 超时时回调的处理函数 
              RT_NULL,                                                      // 超时函数的入口参数 
              10,                                                           // 定时长度
              RT_TIMER_FLAG_PERIODIC);                                      // 周期性定时器 
	
    rt_timer_start(&power_key_det_timer);

    CheckTurnPower();
    
    while(1){
        current_key_val = KeyScan_Result();

//        if(last_key_val != current_key_val){
//            last_key_val = current_key_val;

            KeyProcess(current_key_val);
            current_key_val = 0;
//        }

        rt_thread_delay(15);

        AppTick ++;
        if(AppTick == 5){
            AppTick = 0;
            BatTick ++;

            if((GetBit(PowerOnFlag,Homepage) == 1)&&(GetBit(PowerOnFlag,Menupage) == 0)){
                if(BatTick == 1){
                    regAddr = 40020;
                }else{
                    if(BatTick != 200){    //原先为100
                        regAddr = 40020;
                        BatFlag = 0;
                    }else{
                        regAddr = 40033;
                        BatTick = 0;
                        BatFlag = 1;
                    }
                }
             
                if((EnableRead == 1)&&(Read433Para == 0)){
                    if(writeConfirm == 1){
                        writeConfirm = 0;
                        System.Device.Usart1.WriteNData(MasterBuf,8);
                        delay_ms(20);
                        System.Device.Usart1.WriteNData(MasterBuf,8);
                    }else if(resetConfirm == 1){
                        resetConfirm = 0;
                        System.Device.Usart1.WriteNData(CMD5,8);
                        delay_ms(20);
                        System.Device.Usart1.WriteNData(CMD5,8);
                    }
                    else
                        ModbusMasterRead(SlaverAddr,regAddr,0x03,0x00,0x01);
                }
                
                if(TransmitFail == 0){
                    FailTick++;
                    
                    if(FailTick == 40){
                        FailTick = 0;
                        NonAck = 1;
                    }
                }else if(TransmitFail == 1){
                    FailTick = 0;
                    TransmitFail = 0;
                    NonAck = 0;
                }
            }
        }
    }
}



/**
 * @brief  EXTI line detection callbacks
 * @param  GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{  
    if ( GetKeyState( BUTTON_KEY ) == GPIO_PIN_RESET )
    {
//        User_Printf("This is a key test!\r\n");
        //??????
    }
}

