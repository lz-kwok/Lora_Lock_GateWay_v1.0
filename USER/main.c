#include "system.h"


static rt_thread_t task1 = RT_NULL, task2 = RT_NULL, task3 = RT_NULL;

//重映射串口1到rt_kprintf
void rt_hw_console_output(const char *str)
{
    /* empty console output */
	
	rt_enter_critical();

//	System.Device.Usart6.WriteString((uint8_t *)str);

	rt_exit_critical();
}

/*******************************************************************************
* 描述	    : 初始化数据库
*******************************************************************************/
static void InitData(void)
{
    AppDataPointer = &(App.Data);
    
    AppDataPointer->Adc.A0 = 2400;
    App.Data.Voltage = 42;
    
    ABValue = FlashRead(0);
    if(ABValue > 3)
      ABValue = 1;
    
    TurnoffIndex = FlashRead(1);
    if(TurnoffIndex > 20)
      TurnoffIndex = 1;
    
    InOutValue = FlashRead(2);
    if(InOutValue > 20)
      InOutValue = 1;
}




int main()
{
    InitData();
	//创建线程thread_Fingerprint_Detect_entry
    task1 = rt_thread_create("thread_Fingerprint_Detect_entry", 			   
							thread_Fingerprint_Detect_entry, RT_NULL, 	         
							2048, 											
							1, 												
							30);											

    if (task1 != RT_NULL)                //如果获得线程控制块，启动这个线程
        rt_thread_startup(task1);
    else
        rt_kprintf("create thread_Fingerprint_Detect_entry failed!\r\n");
		
//    //创建线程thread_Menu_show_entry
//    task2 = rt_thread_create("thread_Menu_show_entry", 			            //线程的名称是thread_Menu_show_entry
//                            thread_Menu_show_entry, RT_NULL,                //入口是thread_Menu_show_entry，参数是RT_NULL 
//                            2048,  									        //线程堆栈大小
//                            4, 											    //线程优先级
//                            20);										    //时间片tick
//	
//    if (task2 != RT_NULL) 								                    // 如果获得线程控制块，启动这个线程 
//        rt_thread_startup(task2);
//    else
//        rt_kprintf("create thread_Menu_show_entry failed!\r\n");
//		
//				//创建线程2
//    task3 = rt_thread_create("thread_Key_scan_entry", 			            //线程的名称是thread_Key_scan_entry
//                            thread_Key_scan_entry, RT_NULL,                 //入口是thread_Key_scan_entry，参数是RT_NULL 
//                            512,  									        //线程堆栈大小
//                            3, 											    //线程优先级
//                            20);										    //时间片tick

//    if (task3 != RT_NULL) 								                    // 如果获得线程控制块，启动这个线程 
//        rt_thread_startup(task3);
//    else
//        rt_kprintf("create thread_Key_scan_entry failed!\r\n");

    
	return 0;
}

 



