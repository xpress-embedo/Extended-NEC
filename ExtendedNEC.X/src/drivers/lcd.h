/**
 * @file lcd.h
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Functions.
 */

#ifndef LCD_H
#define	LCD_H

#include "config.h"

#ifdef	__cplusplus
extern "C"
{
#endif

#define LCD_DATA              LATD              /**< LCD Data Lines.*/
#define LCD_DATA_DIR          TRISD             /**< LCD Data Lines Direction.*/
#define LCD_RS                PORTCbits.RC1     /**< LCD RS Pin.*/
#define LCD_RW                PORTCbits.RC0     /**< LCD RW Pin.*/
#define LCD_EN                PORTCbits.RC2     /**< LCD EN Pin.*/
#define LCD_RS_DIR            TRISCbits.TRISC1  /**< LCD RS Pin Direction.*/
#define LCD_RW_DIR            TRISCbits.TRISC0  /**< LCD RW Pin Direction.*/
#define LCD_EN_DIR            TRISCbits.TRISC2  /**< LCD EN Pin Direction.*/

/* LCD Commands */
#define LCD_16x2_INIT         0x38    /**< Initialize 16x2 Lcd in 8-bit Mode.*/
#define LCD_DISP_ON_CUR_ON    0x0E    /**< LCD Display On Cursor On.*/
#define LCD_DISP_ON_CUR_OFF   0x0C    /**< LCD Display On Cursor Off.*/
#define LCD_DISP_ON_CUR_BLNK  0x0F    /**< LCD Display On Cursor Blink.*/
#define LCD_FIRST_ROW         0x80    /**< Move Pointer to First Row.*/
#define LCD_SECOND_ROW        0xC0    /**< Move Pointer to Second Row.*/
#define LCD_CLEAR             0x01    /**< Clear LCD Display.*/

/* LCD Function Prototypes */
void LCD_Init(void);
void LCD_Cmd(u8_t command);
void LCD_Write(u8_t Data);
void LCD_Write_Text(u8_t *msg);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

