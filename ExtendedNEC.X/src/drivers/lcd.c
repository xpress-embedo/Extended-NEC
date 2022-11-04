/**
 * @file lcd.c
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Initialization and Functions Definitions.
 */

#include "lcd.h"

/**
 * @brief Initialize 16x2 LCD Module.
 *
 * Initialize 16x2 LCD Module in 8-bit mode.
 * 
 */
void LCD_Init(void)
{
  LCD_DATA_DIR = 0x00;
  LCD_RS_DIR = 0;
  LCD_RW_DIR = 0;
  LCD_EN_DIR = 0;
  LCD_Cmd(LCD_16x2_INIT);
  __delay_ms(10);
  __delay_ms(10);
  LCD_Cmd(LCD_DISP_ON_CUR_ON);
  __delay_ms(10);
  LCD_Cmd(LCD_DISP_ON_CUR_OFF);
  __delay_ms(10);
  LCD_Cmd(LCD_CLEAR);
  __delay_ms(10);
  LCD_Cmd(LCD_FIRST_ROW);
  __delay_ms(20);
}

/**
 * @brief Send Command to LCD.
 *
 * Send Command to LCD, use the following commands.
 * <b>LCD_16x2_INIT,LCD_DISP_ON_CUR_ON,LCD_DISP_ON_CUR_OFF,LCD_DISP_ON_CUR_BLNK,
 * LCD_FIRST_ROW,LCD_SECOND_ROW,LCD_CLEAR</b>.
 * @param command Command to Send to the LCD.
 */
void LCD_Cmd(u8_t command)
{
  LCD_DATA = command;
  LCD_RS = 0;
  LCD_RW = 0;
  LCD_EN = 1;
  __delay_ms(2);
  LCD_EN = 0;
}

/**
 * @brief Write Data on LCD.
 *
 * Write Data on LCD, specified as arguments.
 * @param Data Data to Write on LCD.
 */
void LCD_Write(u8_t Data)
{
  LCD_DATA = Data;
  LCD_RS = 1;
  LCD_RW = 0;
  LCD_EN = 1;
  __delay_ms(2);
  LCD_EN = 0;
}

/**
 * @brief Write String on LCD.
 *
 * Write String on LCD, specified as arguments.
 * @param *msg First Character Address of the String.
 * @note String Must be terminated by NULL Character.
 */
void LCD_Write_Text(u8_t *msg)
{
  while(*msg)
  {
    LCD_Write(*msg);
    msg++;
  }
}
