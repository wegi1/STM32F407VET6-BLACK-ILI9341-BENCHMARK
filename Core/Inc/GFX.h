/*
 * GFX.h
 *
 *  Created on: 10.09.2019
 *      Author: bolo
 */


#ifndef GFX_H_
#define GFX_H_
//-----------------------------------
#include "ili9341.h"
//------------------------------------

/*--------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------- -----
* Resource definition
---------------------------------------------------------------------------------------------------- ---*/

//-------------------------------
extern RNG_HandleTypeDef hrng;
//-------------------------------


void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color);
void LCD_DisAPoint(u16 x0, u16 y0, u16 Color);
void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color);
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color);

uint16_t LCD_RandColor(void);
void LCD_DrawLine(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_No_Fill_Draw(u16 color, u16 x0, u16 y0, u16 x1, u16 y1);

void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
//-------------------------------
//-------------------------------
#endif /* GFX_H_ */


