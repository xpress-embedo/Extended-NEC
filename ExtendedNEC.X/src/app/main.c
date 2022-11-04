/**
 * @file main.c
 * @author Embedded Laboratory
 * @date April 12, 2016
 * @brief This is the main file of the project.
 * 
 * \mainpage Description
 * TSOP1738 is used to decode the Modulated IR Pulses of Remote Controller, 
 * TSOP1738 Output is connected to micro-controller pin. Extended NEC Protocol
 * State Machine continously monitors this pin for various states and based on 
 * the data present on this pins, address and commands are stored in internal 
 * buffers. These data on reception is displayed on 16x2 LCD.
 * 
 * \section library_sec Libraries Used in this Project
 * \subsection library1 Extended NEC Library
 * This library handles decoding NEC signal.
 * \subsection library2 16x2 LCD Library
 * This library controls and display characters on LCD.
 */

#include "config.h"
#include "lcd.h"
#include "extended_nec.h"

u8_t lcd_line[16] = {0};  /**< LCD Display Buffer.*/

/**
 * Main Program.
 */
void main(void)
{
  u32_t timestamp = 0;
  LCD_Init ();
  Timer0_Init();
  Timer1_Init();
  NEC_Init();
  enable_global_int();
  sprintf(lcd_line,"  Embedded Lab");
  LCD_Cmd (LCD_CLEAR);
  LCD_Write_Text(lcd_line);
  sprintf(lcd_line,"Ext NEC Protocol");
  LCD_Cmd(LCD_SECOND_ROW);
  LCD_Write_Text(lcd_line);
  TRISB &= 0x01;
  LATB &= 0x01;
  while(1)
  {
    if(millis() - timestamp > 500ul )
    {
      if( NEC_Data_Ready() )
      {
        u16_t Address = 0;
        Address = Get_NEC_Address ();
        u16_t Data = 0;
        Data = Get_NEC_Data ();
        sprintf(lcd_line, "Address= 0x%.4X ",Address);
        LCD_Cmd (LCD_FIRST_ROW);
        LCD_Write_Text(lcd_line);
        sprintf(lcd_line, "Data   =   0x%.2X ",Data);
        LCD_Cmd (LCD_SECOND_ROW);
        LCD_Write_Text(lcd_line);
        switch(Data)
        {
          case 0:
            LATB &= 0x01;
            break;
          case 1:
            SET_BIT(LATB,1);
            break;
          case 2:
            SET_BIT(LATB,2);
            break;
          case 3:
            SET_BIT(LATB,3);
            break;
          case 4:
            SET_BIT(LATB,4);
            break;
          case 5:
            SET_BIT(LATB,5);
            break;
          case 6:
            SET_BIT(LATB,6);
            break;
          case 7:
            SET_BIT(LATB,7);
            break;
        }
      }
    }
  }
  return;
}
