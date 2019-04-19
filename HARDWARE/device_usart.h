#ifndef __DEVICE_UART__H
#define __DEVICE_UART__H

#include "system.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#define UART_BufferSize 256

/* User can use this section to tailor USARTx/UARTx instance used and associated resources */
/* Definition for USARTx clock resources */

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA

#define USART1_CLK_ENABLE()              __USART1_CLK_ENABLE();
#define USART1_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART1_FORCE_RESET()             __USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __USART1_RELEASE_RESET()

#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_AF                     GPIO_AF7_USART1

#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()


/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA   

#define USART2_CLK_ENABLE()              __USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define USART2_FORCE_RESET()             __USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __USART2_RELEASE_RESET()

#define USART2_TX_AF                     GPIO_AF7_USART2
#define USART2_RX_AF                     GPIO_AF7_USART2

/* Definition for USART6 Pins */
#define USART6_TX_PIN                    GPIO_PIN_6
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_RX_PIN                    GPIO_PIN_7
#define USART6_RX_GPIO_PORT              GPIOC  

#define USART6_CLK_ENABLE()              __USART6_CLK_ENABLE();
#define USART6_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USART6_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USART6_FORCE_RESET()             __USART6_FORCE_RESET()
#define USART6_RELEASE_RESET()           __USART6_RELEASE_RESET()

#define USART6_TX_AF                     GPIO_AF8_USART6
#define USART6_RX_AF                     GPIO_AF8_USART6

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define USARTx_TX_DMA_STREAM             DMA2_Stream7
#define USARTx_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define USARTx_RX_DMA_STREAM             DMA2_Stream2

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn               DMA2_Stream7_IRQn
#define USARTx_DMA_RX_IRQn               DMA2_Stream2_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA2_Stream2_IRQHandler
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

#define RECEIVELEN 			        256  
#define USART_DMA_SENDING 		    1    //?????
#define USART_DMA_SENDOVER 		    0    //????



typedef struct  
{  
	uint8_t receive_flag:1;		     //??????
	uint8_t dmaSend_flag:1;		     //????????
	uint16_t rx_len;		     //????
	uint8_t usartDMA_rxBuf[RECEIVELEN];  //DMA????
}USART_RECEIVETYPE;  
   
extern USART_RECEIVETYPE UsartType1;  
extern rt_mutex_t mutex;
 
/* Exported variables --------------------------------------------------------*/
extern volatile uint8_t UART_RxBuffer[UART_BufferSize];
extern volatile uint8_t UART_TxBuffer[UART_BufferSize];
extern const uint32_t Usart_BaudRate;
extern UART_HandleTypeDef UartHandle;



/* Exported functions ------------------------------------------------------- */
void uart1_init(uint32_t Usart_BaudRate);
void uart2_init(uint32_t Usart_BaudRate);
void uart6_init(u32 Usart_BaudRate);
void Uart1000Routine(void);
uint8_t *JudgeStr(uint16_t waittime);
void thread_uartDMA_receive_entry(void* parameter);

#endif
