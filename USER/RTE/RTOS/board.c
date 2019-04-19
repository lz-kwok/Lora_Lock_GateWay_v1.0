#include <rthw.h>
#include <rtthread.h>
#include "system.h"

// rtthread tick configuration
// 1. include header files
// 2. configure rtos tick and interrupt
// 3. add tick interrupt handler

// rtthread tick configuration
// 1. include some header file as need
#include <stm32f4xx.h>

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN    (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#define HEAP_BEGIN    (__segment_end("HEAP"))
#else
extern int __bss_end;
#define HEAP_BEGIN    (&__bss_end)
#endif

#define SRAM_SIZE         96
#define SRAM_END          (0x20000000 + SRAM_SIZE * 1024)


/**
 * This function will initial STM32 board.
 */
static uint32_t CpuID[3]; 
static void GetLockCode(void) 
{ 
  //获取CPU唯一ID 
  CpuID[0]=*(uint32_t*)(0x1ffff7e8); 
  CpuID[1]=*(uint32_t*)(0x1ffff7ec); 
  CpuID[2]=*(uint32_t*)(0x1ffff7f0); 
  //加密算法,很简单的加密算法 
  Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3); 
}




extern uint8_t OSRunning;


void rt_hw_board_init()
{   
    HAL_Init();                  	    //初始化HAL库   
    HSI_Clock_Init(72);               //设置时钟,72Mhz	
	// rtthread tick configuration
	// 2. Configure rtos tick and interrupt
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    
	AppDataPointer = &(App.Data);
	delay_init(72);

	Gpio_init(); 
//    Key_init(BUTTON_KEY, BUTTON_MODE_GPIO);
//    Adc_init();
//    IIC1_Init();
	uart1_init(9600);
    uart2_init(57600);
	uart6_init(115200);
//    SPI1_init(256);
#ifdef USB_VCP
	USB_Vcp_Init();
#endif
#ifdef SOC_RTC
	Rtc_Init();
#endif
#ifdef BME280_MODUOLE
	BME280_Init();   
#endif
//    GUI_Initialize();
//    GetLockCode();

//上面为硬件初始化
	
	OSRunning=1;

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
		rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void*)HEAP_BEGIN, (void*)SRAM_END);
#endif
}

// rtthread tick configuration
// 3. add tick interrupt handler 
void SysTick_Handler(void)
 {
 	/* enter interrupt */
 	rt_interrupt_enter();
	HAL_IncTick();
 	rt_tick_increase();
 
 	/* leave interrupt */
 	rt_interrupt_leave();
 }
