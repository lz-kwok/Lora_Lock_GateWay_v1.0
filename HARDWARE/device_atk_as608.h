#ifndef __DEVICE_ATK_AS608__H
#define __DEVICE_ATK_AS608__H

#include "system.h"

/* Exported defines ----------------------------------------------------------*/
/* Exported defines ----------------------------------------------------------*/
#define AS608Addr                   0XFFFFFFFF
#define CharBuffer1 				0x01
#define CharBuffer2 				0x02


typedef struct
{
	uint16_t PS_max;		//指纹最大容量
	uint8_t  PS_level;		//安全等级
	uint32_t PS_addr;
	uint8_t  PS_size;		//通讯数据包大小
	uint8_t  PS_N;			//波特率基数N
}SysPara;

typedef struct  
{
	uint16_t pageID;		//指纹ID
	uint16_t mathscore;		//匹配得分
}SearchResult;

typedef enum
{
	handshake_pre = 0,
	handshake_judging,
	handshake_done,
	handle_data,

}ATK_AS_STA;

   

/* Exported variables --------------------------------------------------------*/
extern char handshake_status;



/* Exported functions ------------------------------------------------------- */
void thread_Fingerprint_Detect_entry(void* parameter);

#endif


