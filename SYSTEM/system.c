/*******************************************************************************
* File_Name : system.c
* Function  : ϵͳӦ���ļ�
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

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��


#if SYSTEM_SUPPORT_OS	           //���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).

//֧��RT-Thread
extern volatile rt_uint8_t rt_interrupt_nest;

//��board.c�ļ���rt_hw_board_init()���潫����Ϊ1
uint8_t OSRunning=0;

#ifdef 	RT_THREAD_PRIORITY_MAX					         	//RT_THREAD_PRIORITY_MAX������,˵��Ҫ֧��RT-Thread	
#define delay_osrunning		  OSRunning			      	//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	RT_TICK_PER_SECOND		//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	rt_interrupt_nest			//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif


//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{
#ifdef RT_THREAD_PRIORITY_MAX
	 rt_enter_critical();
#endif	
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{	
#ifdef RT_THREAD_PRIORITY_MAX
	  rt_exit_critical();
#endif	
}

//����OS�Դ�����ʱ������ʱ
//ticks:��ʱ�Ľ�����
void delay_ostimedly(u32 ticks)
{
#ifdef RT_THREAD_PRIORITY_MAX
	  rt_thread_delay(ticks);
#endif	
}
 
#endif
 
 
			   
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ��
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 																	//�����Ҫ֧��OS.
	u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
	fac_us=SYSCLK;																				//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
#if SYSTEM_SUPPORT_OS 																	//�����Ҫ֧��OS.
	reload=SYSCLK;																				//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/delay_ostickspersec;									//����delay_ostickspersec�趨���ʱ��
																												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����	
	fac_ms=1000/delay_ostickspersec;											//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   						//����SYSTICK�ж�
	SysTick->LOAD=reload; 																//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 							//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;															//��OS��,����ÿ��ms��Ҫ��systickʱ����   
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

#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=nus*fac_us; 								//��Ҫ�Ľ����� 
	delay_osschedlock();							//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;					//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();						//�ָ�OS����											    
}  
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)													//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);				//OS��ʱ
		}
		nms%=fac_ms;														//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));								//��ͨ��ʽ��ʱ
}
#else  
//����osʱ
//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//ʱ�����	  		 
	SysTick->VAL=0x00;        				//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 	//��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));			//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 	//�رռ�����
	SysTick->VAL =0X00;       								//��ռ����� 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��168M������,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           			//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));							//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       		//�رռ�����
	SysTick->VAL =0X00;     		  										//��ռ�����	  	    
} 
//��ʱnms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//������540,�ǿ��ǿ��ܳ�Ƶʹ��,
																//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
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
* ������  : HSE_Clock_Init
* ����    : ��ʼ���ⲿ������Ϊ��ʱ��Դ
* �������: MainFreq
* ���ز���: ��
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
* ������  : HSI_Clock_Init
* ����    : ��ʼ���ڲ�������Ϊ��ʱ��Դ
* �������: MainFreq
* ���ز���: ��
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
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
