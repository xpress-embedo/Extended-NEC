/**
 * @file config.c
 * @author Embedded Laboratory
 * @date April 30, 2016
 * @brief Configuration File of the Project.
 *
 * This file controls the speed and various other important things associated 
 * with micro-controller.
 */

#include "config.h"
#include "extended_nec.h"

volatile u32_t t0_millis = 0;     /**< Milli-Second Counter.*/
Version_s SoftVer = {1,0,0,1UL};  /**< Software Version.*/

/*
// This type method is not supported by xc8 (because it doesn't follow C99 Standards)
{
  .major = 1,
  .minor = 1,
  .build = 4,
  .fix = 10UL
};
*/

/**
 * @brief Interrupt Service Routine (High).
 *
 * Interrupt Service Routine for high priority interrupts.
 * 
 * @note Call to this function is done automatically whenever the high priority
 * interrupt occurs and its corresponding flag gets set.
 */
void interrupt ISR_Code(void)
{
  if ( TMR1IF == 1 )
  {
    TMR1IF = 0;
    TMR1H	 = 0xFE;
    TMR1L	 = 0xA2;
    NEC_State_Machine();
  }
  if( TMR0IF == 1 )
  {
    TMR0IF = 0;
    TMR0H	 = 0xEC;
    TMR0L	 = 0x78;
    t0_millis++;
  }
}

/**
 * @brief Initialize Timer-0 Module.
 *
 * Initialize Timer-0 of PIC micro-controller for generating 1ms interrupt.
 */
void Timer0_Init(void)
{
  T0CON  = 0x88;
  TMR0IF = 0;
  TMR0H  = 0xF4;
  TMR0L  = 0x48;
  TMR0IE = 1;
}

/**
 * @brief Initialize Timer-1 Module.
 *
 * Initialize Timer-1 of PIC micro-controller for generating 70us interrupt.
 */
void Timer1_Init()
{
  T1CON  = 0x01;
  TMR1IF = 0;
  //Prescaler 1:1; TMR1 Preload = 61336; Actual Interrupt Time : 70 us
  TMR1H  = 0xFE;
  TMR1L  = 0xA2;
  TMR1IE = 1;
  PEIE   = 1;
}

/**
 * @brief Millis.
 *
 * Returns the number of milliseconds since the board began running the current 
 * program. This number will overflow (go back to zero), after approximately 
 * 50 days.This function returns time in milli-second
 * @return Number of milliseconds since the program started (#u32_t)
 */
u32_t millis(void)
{
  return t0_millis;
}


/**
 * @brief Copy RAM
 * 
 * This function copies ram to ram.
 * This function is not safe, it will clobber its own working parts if the stack
 * is included in the swath of the destination RAM
 * Designed for pSrc = First address to be copied,Size = number of bytes to copy
 * 
 * @param *pSrc   Address of the first byte of Source data.
 * @param *pDest  Address of the first byte of Destination data.
 * @param size    Size of data which is to be copied from source to destination.
 */
void Copy_RAM(u8_t *pSrc, u8_t *pDest, u8_t size)
{
  for ( ; size; size--, pSrc++, pDest++) // Loop until size = 0
    *pDest = *pSrc;                      // Copy source to destination
}
