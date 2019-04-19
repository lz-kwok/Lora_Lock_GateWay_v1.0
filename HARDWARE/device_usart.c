/*******************************************************************************
* File_Name : device_uart.c
* Function  : 串口设备初始化
* V0.1    Guolz         2018/3/6       
*******************************************************************************/
#include "device_usart.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "rtthread.h"					//os 使用	  
#endif

rt_mutex_t mutex = RT_NULL;

/* Private variables ---------------------------------------------------------*/
uint8_t UART1_RxBuffer[UART_BufferSize];
uint8_t UART1_TxBuffer[UART_BufferSize];
uint8_t UART2_RxBuffer[UART_BufferSize];
uint8_t UART2_TxBuffer[UART_BufferSize];
uint8_t UART6_RxBuffer[UART_BufferSize];
uint8_t UART6_TxBuffer[UART_BufferSize];

USART_RECEIVETYPE UsartType1;

static char handshake_status = handshake_pre;



//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	System.Device.Usart6.WriteData(ch);      
	return ch;
}
#endif 



void uart1_SendByte(uint8_t data)
{
    HAL_UART_Transmit(&Uart1Handle, &data, 1, 1000);
}

void uart1_SendNByte(uint8_t *data,uint16_t len)
{
    HAL_UART_Transmit(&Uart1Handle, data, len, 1000);
}

void uart1_SendString(uint8_t *s)
{
    while(*s != '\0')
    {
        HAL_UART_Transmit(&Uart1Handle, s, 1, 1000);
        s++;
    }
}

void uart1_SendData_DMA(uint8_t *pdata, uint16_t Length)  
{  
//    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    while(UsartType1.dmaSend_flag == USART_DMA_SENDING);  
    UsartType1.dmaSend_flag = USART_DMA_SENDING;  
    HAL_UART_Transmit_DMA(&Uart1Handle, pdata, Length);  
//    rt_mutex_release(mutex);
} 

//初始化IO 串口1 
//bound:波特率
void uart1_init(u32 Usart_BaudRate)
{	
	//UART1 初始化设置
	GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO TX/RX clock */
    USART1_TX_GPIO_CLK_ENABLE();
    USART1_RX_GPIO_CLK_ENABLE();
    
    /* Enable USART1 clock */
    USART1_CLK_ENABLE();
    /* Enable DMA1 clock */
    DMAx_CLK_ENABLE();   
    
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USART1_TX_AF;
    
    HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART1_RX_PIN;
    GPIO_InitStruct.Alternate = USART1_RX_AF;
    
    HAL_GPIO_Init( USART1_RX_GPIO_PORT, &GPIO_InitStruct );
    
    /*##-3- Configure the DMA streams ##########################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = USARTx_TX_DMA_STREAM;
    
    hdma_tx.Init.Channel             = USARTx_TX_DMA_CHANNEL;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
    
    HAL_DMA_Init(&hdma_tx);   
    
    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(&Uart1Handle, hdmatx, hdma_tx);
      
    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance                 = USARTx_RX_DMA_STREAM;
    
    hdma_rx.Init.Channel             = USARTx_RX_DMA_CHANNEL;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
    hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
    hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4; 

    HAL_DMA_Init(&hdma_rx);
      
    /* Associate the initialized DMA handle to the the UART handle */
    __HAL_LINKDMA(&Uart1Handle, hdmarx, hdma_rx);
          
    HAL_UART_Receive_DMA(&Uart1Handle, UsartType1.usartDMA_rxBuf, RECEIVELEN);  
    __HAL_UART_ENABLE_IT(&Uart1Handle, UART_IT_IDLE);  
    
    
    /*##-1- Configure the UART peripheral ######################################*/
    
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART1 configured as follow:
            - Word Length = 8 Bits
            - Stop Bit = One Stop bit
            - Parity = None
            - BaudRate = 'Usart_BaudRate' baud
            - Hardware flow control disabled (RTS and CTS signals) */
    Uart1Handle.Instance        = USART1;
    Uart1Handle.Init.BaudRate   = Usart_BaudRate;
    Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart1Handle.Init.StopBits   = UART_STOPBITS_1;
    Uart1Handle.Init.Parity     = UART_PARITY_NONE;
    Uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    Uart1Handle.Init.Mode       = UART_MODE_TX_RX;
    
    if ( HAL_UART_Init( &Uart1Handle ) != HAL_OK )
    {
      while( 1 );
    }
    
//    Uart1Handle.pRxBuffPtr = ( uint8_t * )UART1_RxBuffer;
//    Uart1Handle.RxXferSize = UART_BufferSize;
//    Uart1Handle.ErrorCode  = HAL_UART_ERROR_NONE;
    
    /*##-4- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
    HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);
      
    /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
    HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 2, 1);   
    HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);
    
    /* NVIC for USART1 */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    
//    if(HAL_UART_Receive_IT(&Uart1Handle,UART1_RxBuffer,1)!=HAL_OK)
//      Error_Handler();
    
    
#ifdef Printf2Uart1
//    am_util_stdio_printf_init((am_util_stdio_print_char_t)
//    USART1_SendString);  
#endif
    
    System.Device.Usart1.WriteData   = uart1_SendByte; 
    System.Device.Usart1.WriteString = uart1_SendString; 
    System.Device.Usart1.WriteNData  = uart1_SendNByte; 
    System.Device.Usart1.WriteDMAData = uart1_SendData_DMA;
}

void uart2_SendByte(uint8_t data)
{
    HAL_UART_Transmit(&Uart2Handle, &data, 1, 1000);
}

void uart2_SendNByte(uint8_t *data,uint16_t len)
{
    HAL_UART_Transmit(&Uart2Handle, data, len, 1000);
}

void uart2_SendString(uint8_t *s)
{
    while(*s != '\0')
    {
        HAL_UART_Transmit(&Uart2Handle, s, 1, 1000);
        s++;
    }
}

void uart2_init(u32 Usart_BaudRate)
{	
	//UART2 初始化设置
	GPIO_InitTypeDef GPIO_InitStruct;
   
    /* Enable GPIO TX/RX clock */
    USART2_TX_GPIO_CLK_ENABLE();
    USART2_RX_GPIO_CLK_ENABLE();
    
    /* Enable USART2 clock */
    USART2_CLK_ENABLE();
     
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USART2_TX_AF;
    
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USART2_RX_PIN;
    GPIO_InitStruct.Alternate = USART2_RX_AF;
    
    HAL_GPIO_Init( USART2_RX_GPIO_PORT, &GPIO_InitStruct );
    
    /*##-1- Configure the UART peripheral ######################################*/
    
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART2 configured as follow:
            - Word Length = 8 Bits
            - Stop Bit = One Stop bit
            - Parity = None
            - BaudRate = 'Usart_BaudRate' baud
            - Hardware flow control disabled (RTS and CTS signals) */
    Uart2Handle.Instance        = USART2;
    Uart2Handle.Init.BaudRate   = Usart_BaudRate;
    Uart2Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart2Handle.Init.StopBits   = UART_STOPBITS_1;
    Uart2Handle.Init.Parity     = UART_PARITY_NONE;
    Uart2Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    Uart2Handle.Init.Mode       = UART_MODE_TX_RX;
    
    if ( HAL_UART_Init( &Uart2Handle ) != HAL_OK )
    {
      while( 1 );
    }
    
    Uart2Handle.pRxBuffPtr = ( uint8_t * )UART2_RxBuffer;
    Uart2Handle.RxXferSize = UART_BufferSize;
    Uart2Handle.ErrorCode  = HAL_UART_ERROR_NONE;
    
    /* NVIC for USART1 */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    
    if(HAL_UART_Receive_IT(&Uart2Handle,UART2_RxBuffer,1)!=HAL_OK)
      Error_Handler();
   
#ifdef Printf2Uart2
//       am_util_stdio_printf_init((am_util_stdio_print_char_t)
//    USART2_SendString);  
#endif
    
    System.Device.Usart2.WriteData = uart2_SendByte; 
    System.Device.Usart2.WriteString = uart2_SendString; 
}


/* 线程thread_uartDMA_receive_entry的入口函数 */
void thread_uartDMA_receive_entry(void* parameter)
{

	
	while (1)
	{
//		if(UsartType1.receive_flag)			//产生空闲中断
//        {  
//            UsartType1.receive_flag=0;			//清理标记
//            
//        }

	}
}


uint8_t *JudgeStr(uint16_t waittime)
{
	char *data;
	uint8_t str[8];
	str[0]=0xef;					
	str[1]=0x01;
	str[2]=(uint8_t)(AS608Addr>>24);	
	str[3]=(uint8_t)(AS608Addr>>16);		
	str[4]=(uint8_t)(AS608Addr>>8);	
 	str[5]=(uint8_t)AS608Addr;				
	str[6]=0x07;					
	str[7]='\0';

	while(--waittime)
	{
		rt_thread_delay(1);
		if(handshake_status == handle_data)			//接收到一次数据
		{
			handshake_status = handshake_done;
			rt_thread_delay(100);                 	//等待后续接收完成
			
			data=strstr((const char*)rx2Buff,(const char*)str);
			Uart2lenth = 0;
			
			if(data)
				return (uint8_t *)data;	
		}
	}
	
	return 0;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{  
    if(UartHandle->Instance==USART2){		
        rx2Buff[Uart2lenth++] = UART2_RxBuffer[0];
        //判断是不是模块返回的应答包	
        if(rx2Buff[0]==0xEF&&rx2Buff[1]==0x01&&rx2Buff[6]==0X07){
			if(handshake_status == handshake_pre){
                Uart2lenth = 0;
                handshake_status = handshake_judging;
			}else if(handshake_status == handshake_done){
				handshake_status = handle_data;
			}

        }
    }
}

void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
    uint32_t temp;  
  
    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->NDTR;  
        UsartType1.rx_len =  RECEIVELEN - temp;   
        UsartType1.receive_flag = 1;  
        HAL_UART_Receive_DMA(huart,UsartType1.usartDMA_rxBuf,RECEIVELEN);  
    }  
} 

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uartHandle)
{
    __HAL_DMA_DISABLE(uartHandle->hdmatx);  
    UsartType1.dmaSend_flag = USART_DMA_SENDOVER;  
}

void USARTx_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Uart1Handle.hdmarx);
}

void USARTx_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(Uart1Handle.hdmatx);
}
/**
 * @}
 */
void USART1_IRQHandler(void)  
{  
    EnterCritical();
    UsartReceive_IDLE(&Uart1Handle);
    HAL_UART_IRQHandler(&Uart1Handle);
    ExitCritical();
//    HAL_UART_Receive_IT(&Uart1Handle,UART1_RxBuffer,1);       
} 

/**
 * @}
 */
void USART2_IRQHandler(void)  
{  
    HAL_UART_IRQHandler(&Uart2Handle);
    HAL_UART_Receive_IT(&Uart2Handle,UART2_RxBuffer,1);
} 

