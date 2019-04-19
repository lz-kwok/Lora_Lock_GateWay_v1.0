#ifndef   _DEVICE_PRINTF_H_
#define   _DEVICE_PRINTF_H_




//以下是串口函数接口


typedef void (*am_util_stdio_print_char_t)(char *pcStr);
void am_util_stdio_printf_init(am_util_stdio_print_char_t pfnCharPrint);
uint32_t User_Printf(char *pcFmt, ...);



#endif
