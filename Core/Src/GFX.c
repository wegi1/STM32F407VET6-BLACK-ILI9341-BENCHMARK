/*
 * GFX.c
 *
 *  Created on: 10.09.2019
 *      Author: bolo
 */


#include "main.h"
#include "GFX.h"
#include "stdlib.h"

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/

//-------------------------------------------------------------------------------------------
volatile uint32_t OS_TimeMS = 0; //!!! delete and declare as extern ! when use STemWIN !
//-------------------------------------------------------------------------------------------
#define  AHB1_ENABLED_DEVICES_LIST  RCC_AHB1Periph_GPIOA | \
        RCC_AHB1Periph_GPIOB | \
        RCC_AHB1Periph_GPIOC | \
        RCC_AHB1Periph_GPIOD | \
        RCC_AHB1Periph_GPIOE | \
        RCC_AHB1Periph_GPIOH | \
        RCC_AHB1Periph_CRC     // emWin requires CRC

#define FSMC_PORT_D_PINS_LIST       GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|   \
        GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|   \
        GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_13| \
        GPIO_Pin_14|GPIO_Pin_15

#define FSMC_PORT_E_PINS_LIST       GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|    \
        GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12| \
        GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15
//******************************************************************************************************

//-------------------------------------
uint16_t LCD_RandColor(void)
{
    return HAL_RNG_GetRandomNumber(&hrng)&0x0000FFFF;
}
//------------------------------------
void LCD_DrawLine(u16 color, u16 x1, u16 y1, u16 x2, u16 y2)
{
    if(y1 == y2)
    {
        LCD_DisARectangular(x1,y1,x2,y2,color);
        return;
    }

    if(x1 == x2)
    {
        LCD_DisARectangular(x1,y1,x2,y2,color);
        return;
    }

    int steep = abs(y2-y1)>abs(x2-x1);
    if (steep)
    {
        swap(x1,y1);
        swap(x2,y2);
    }
    if(x1>x2)
    {
        swap(x1,x2);
        swap(y1,y2);
    }
    int dx,dy;
    dx=x2-x1;
    dy=abs(y2-y1);
    int err=dx/2;
    int ystep;
    if(y1<y2)   ystep = 1;
    else        ystep = -1;

    for (;x1<=x2;x1++)
    {
        if (steep)        LCD_DisAPoint(y1,x1,color);
        else LCD_DisAPoint(x1,y1,color);
        err-=dy;
        if (err<0)
        {
            y1 += ystep;
            err+=dx;
        }
    }
}
//���������������������E
//------------------------------------

//------------------------------------


/*--------------------------------------------------------------------*/





/********************************************************************************************************
 *  Function: LCD_DisALoop
 *  Object: Display a loop
 *  Input: site,radius and color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisALoop(u16 x0, u16 y0, u8 r, u16 Color)
{
    s16 a,b,next;
    a	=	0;
    b = r;
    next = 3 - (r<<1);
    while(a <= b)
    {
        LCD_DisAPoint(x0+a, y0-b, Color);
        LCD_DisAPoint(x0+b, y0-a, Color);
        LCD_DisAPoint(x0+b, y0+a, Color);
        LCD_DisAPoint(x0+a, y0+b, Color);
        LCD_DisAPoint(x0-a, y0+b, Color);
        LCD_DisAPoint(x0-b, y0+a, Color);
        LCD_DisAPoint(x0-a, y0-b, Color);
        LCD_DisAPoint(x0-b, y0-a, Color);
        a++;
        //use the bresenham
        if(next<0)
            next += 4*a+6;
        else
        {
            next += 10+4*(a-b);
            b--;
        }
    }
}
/********************************************************************************************************
 *  Function: LCD_DisAPoint
 *  Object: Display a point at screen
 *  Input: site and color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisAPoint(u16 x0, u16 y0, u16 Color)
{

    LCD->LCD_REG = (ILI_CASET);
    LCD->LCD_RAM = (y0>>8);
    LCD->LCD_RAM = (y0);
    LCD->LCD_RAM = (y0>>8);
    LCD->LCD_RAM = (y0);
    LCD->LCD_REG = (ILI_PASET);
    LCD->LCD_RAM = (x0>>8);
    LCD->LCD_RAM = (x0);
    LCD->LCD_RAM = (x0>>8);
    LCD->LCD_RAM = (x0);
    LCD->LCD_REG = (ILI_RAMWR);
    LCD->LCD_RAM = (Color);
}
/********************************************************************************************************
 *  Function: LCD_DisASquare
 *  Object: Display a square
 *  Input: start point, wide, color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisASquare(u16 x0, u16 y0, u16 wide, u16 Color)
{
    u32 i;
    LCD_OpenWin(x0, y0, x0+wide-1, y0+wide-1);
    for(i = 0; i < (wide*wide); i++)
        LCD->LCD_RAM = Color;
}
/********************************************************************************************************
 *  Function: LCD_DisARectangular
 *  Object: Display a rectangular
 *  Input: start point, end point, color
 *  Output: none
 *  brief: none
 ********************************************************************************************************/
void LCD_DisARectangular(u16 x0, u16 y0, u16 x1, u16 y1, u16 Color)
{
    u32 i;
    u16 x00, x01, y00, y01;

    if(x0 < x1) {x00 = x0; x01 = x1;}
    else {x00 = x1; x01 = x0;}

    if(y0 < y1) {y00 = y0; y01 = y1;}
    else {y00 = y1; y01 = y0;}

    LCD_OpenWin(x00, y00, x01, y01);

    for(i = 0; i <= ((1+x01-x00)*(1+y01-y00)); i++) {LCD->LCD_RAM = Color;}
}

/*******************************************************************************************************
 * drawing no fill rectangle
 ******************************************************************************************************/
void LCD_No_Fill_Draw(u16 color, u16 x0, u16 y0, u16 x1, u16 y1)
{
    LCD_DrawLine(color,x0,y0,x1,y0);
    LCD_DrawLine(color,x0,y1,x1,y1);
    LCD_DrawLine(color,x0,y0,x0,y1);
    LCD_DrawLine(color,x1,y0,x1,y1);
}
//*******************************************************************************************************
//*******************************************************************************************************

/**************************************************************************/
/*!
   @brief    Draw a circle with filled color
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    color 16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    writeFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}
/**************************************************************************/
/*!
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
 */
/**************************************************************************/
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color)
{

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) writeFastVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) writeFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) writeFastVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) writeFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}
/*****************************************
 * help function to write H | V lines
 *****************************************/
void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_DrawLine(color, x, y, x, y+h-1);
}
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_DrawLine(color, x, y, x+w-1, y);
}
/*******************************************************************************************************
 * end of screen functions
 *******************************************************************************************************/




//----------------------

//**************************************************************************************************************
//**************************************************************************************************************
//**************************************************************************************************************


