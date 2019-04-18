#ifndef _LQOLED_H
#define _LQOLED_H

#include "system.h"


//汉字大小，英文数字大小
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3


//-----------------OLED端口定义----------------  					   


void LCD_TEST(void);
void InitLCD(void);
void LCD_CLS(void);
void LCD_CLS_y(char y);
void LCD_CLS_line_area(uint8_t start_x,uint8_t start_y,uint8_t width);
void LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F6x8);
void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *ch,uint8_t mode);
void LCD_P14x16Str(uint8_t x,uint8_t y,uint8_t *ch,uint8_t mode);
void LCD_P16x16Str(uint8_t x,uint8_t y,uint8_t *ch,uint8_t mode);
//extern void LCD_Print(u8 x, u8 y, u8 *ch);
void LCD_PutPixel(uint8_t x,uint8_t y);
void LCD_Print(uint8_t x,uint8_t y,uint8_t *ch);
void LCD_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
void Draw_BMP(uint8_t x,uint8_t y,const uint8_t *bmp,uint8_t bmp_len); 
void LCD_Fill(uint8_t dat);
void ClearLcdLine(uint8_t line);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Refresh_Gram(void);
void PutHanzi(uint8_t x,uint8_t y,uint8_t *ch);




#endif

