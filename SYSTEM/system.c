/*******************************************************************************
* File_Name : system.c
* Function  : 系统应用文件
* V0.1    Guolz         2018/3/6       
*******************************************************************************/

#include "system.h"

DataStruct * AppDataPointer;
AppStruct App;

#if SYSTEM_SUPPORT_OS
#include "rtthread.h"					
#endif

static const u8 HextoAscii[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数


#if SYSTEM_SUPPORT_OS	           //如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).

//支持RT-Thread
extern volatile rt_uint8_t rt_interrupt_nest;

//在board.c文件的rt_hw_board_init()里面将其置为1
uint8_t OSRunning=0;

#ifdef 	RT_THREAD_PRIORITY_MAX					         	//RT_THREAD_PRIORITY_MAX定义了,说明要支持RT-Thread	
#define delay_osrunning		  OSRunning			      	//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	RT_TICK_PER_SECOND		//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	rt_interrupt_nest			//中断嵌套级别,即中断嵌套次数
#endif


//us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void)
{
#ifdef RT_THREAD_PRIORITY_MAX
	 rt_enter_critical();
#endif	
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{	
#ifdef RT_THREAD_PRIORITY_MAX
	  rt_exit_critical();
#endif	
}

//调用OS自带的延时函数延时
//ticks:延时的节拍数
void delay_ostimedly(u32 ticks)
{
#ifdef RT_THREAD_PRIORITY_MAX
	  rt_thread_delay(ticks);
#endif	
}
 
#endif
 
 
			   
//初始化延迟函数
//SYSTICK的时钟固定为AHB时钟
//SYSCLK:系统时钟频率
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 																	//如果需要支持OS.
	u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	fac_us=SYSCLK;																				//不论是否使用OS,fac_us都需要使用
#if SYSTEM_SUPPORT_OS 																	//如果需要支持OS.
	reload=SYSCLK;																				//每秒钟的计数次数 单位为M	   
	reload*=1000000/delay_ostickspersec;									//根据delay_ostickspersec设定溢出时间
																												//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	fac_ms=1000/delay_ostickspersec;											//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//开启SYSTICK中断
	SysTick->LOAD=reload; 																//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 							//开启SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;															//非OS下,代表每个ms需要的systick时钟数   
#endif
}								    

u8 HexToAscii(u8 hex)
{
//    Assert(hex < 16);

    if (hex < 16) 
        return HextoAscii[hex];
    else 
         return 0x30;
}

#if SYSTEM_SUPPORT_OS 						//如果需要支持OS.
//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 								//需要的节拍数 
	delay_osschedlock();							//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;					//时间超过/等于要延迟的时间,则退出.
		}  
	};
	delay_osschedunlock();						//恢复OS调度											    
}  
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)	    
	{		 
		if(nms>=fac_ms)													//延时的时间大于OS的最少时间周期 
		{ 
   			delay_ostimedly(nms/fac_ms);				//OS延时
		}
		nms%=fac_ms;														//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((u32)(nms*1000));								//普通方式延时
}
#else  
//不用os时
//延时nus
//nus为要延时的us数.	
//注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//时间加载	  		 
	SysTick->VAL=0x00;        				//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 	//关闭计数器
	SysTick->VAL =0X00;       								//清空计数器 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对168M条件下,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           			//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));							//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//关闭计数器
	SysTick->VAL =0X00;     		  										//清空计数器	  	    
} 
//延时nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//这里用540,是考虑可能超频使用,
																//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif

/*******************************************************************************
* 函数名  : HSE_Clock_Init
* 描述    : 初始化外部晶振作为主时钟源
* 输入参数: MainFreq
* 返回参数: 无
*******************************************************************************/
void HSE_Clock_Init(uint8_t MainFreq)
{
    static uint32_t PLLM = 0,PLLN = 0,PLLP = 0,PLLQ = 0;

    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* -1- Select HSI as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -2- Enable HSE Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    
#ifdef HSE_CRYSTAL  
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
#elif defined (HSE_BYPASS)
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
#endif /* HSE_CRYSTAL */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    
    switch(MainFreq)
    {
        case 48 :  
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV8; PLLQ = 8;   //48MHz
          break;        
        case 72:  
          PLLM = (HSE_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz
          break;        
        case 80:  
          PLLM = (HSE_VALUE/2000000); PLLN = 240; PLLP = RCC_PLLP_DIV6; PLLQ = 10;  //80MHz
          break;        
        case 96:  
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break; 
        case 108:
          PLLM = (HSE_VALUE/2000000); PLLN = 216; PLLP = RCC_PLLP_DIV4; PLLQ = 9;   //108MHz
          break;
        case 144:  
          PLLM = (HSE_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV2; PLLQ = 6;   //144MHz
          break;
        default:
          PLLM = (HSE_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break;        
    }
        
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
    
    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -4- Optional: Disable HSI Oscillator (if the HSI is no more needed by the application) */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
}


/*******************************************************************************
* 函数名  : HSI_Clock_Init
* 描述    : 初始化内部晶振作为主时钟源
* 输入参数: MainFreq
* 返回参数: 无
*******************************************************************************/
void HSI_Clock_Init(uint8_t MainFreq)
{
    static uint32_t PLLM = 0,PLLN = 0,PLLP = 0,PLLQ = 0;
  
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    
    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
    
    /* -1- Select HSE as system clock source to allow modification of the PLL configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    
    switch(MainFreq)
    {
        case 16 : 
          PLLM = (HSI_VALUE/1000000); PLLN = 96;  PLLP = RCC_PLLP_DIV6; PLLQ = 2;   //16MHz
          break; 
        case 24:
          PLLM = (HSI_VALUE/2000000); PLLN = 96;  PLLP = RCC_PLLP_DIV8; PLLQ = 4;   //24MHz
          break; 
        case 48 :  
          PLLM = (HSI_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV8; PLLQ = 8;   //48MHz
          break;        
        case 72:  
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz
          break;        
        case 80:  
          PLLM = (HSI_VALUE/2000000); PLLN = 240; PLLP = RCC_PLLP_DIV6; PLLQ = 10;  //80MHz
          break;        
        case 96:  
          PLLM = (HSI_VALUE/2000000); PLLN = 192; PLLP = RCC_PLLP_DIV4; PLLQ = 8;   //96MHz
          break; 
        case 108:
          PLLM = (HSI_VALUE/2000000); PLLN = 216; PLLP = RCC_PLLP_DIV4; PLLQ = 9;   //108MHz
          break;
        case 144:  
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV2; PLLQ = 6;   //144MHz
          break;
        default:
          PLLM = (HSI_VALUE/2000000); PLLN = 144; PLLP = RCC_PLLP_DIV4; PLLQ = 6;   //72MHz   
          break;        
    }
    
    RCC_OscInitStruct.PLL.PLLM = PLLM;
    RCC_OscInitStruct.PLL.PLLN = PLLN;
    RCC_OscInitStruct.PLL.PLLP = PLLP;
    RCC_OscInitStruct.PLL.PLLQ = PLLQ;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    /* -3- Select the PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType         = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource      = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider     = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider    = RCC_HCLK_DIV2;  
    RCC_ClkInitStruct.APB2CLKDivider    = RCC_HCLK_DIV1;  
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
      Error_Handler();
    }
    
    /* -4- Optional: Disable HSE Oscillator (if the HSE is no more needed by the application) */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
}

SystemStruct System;

void Error_Handler( void )
{
  while (1)
  {}
}

#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
