/**
 * @file extended_nec.h
 * @author Embedded Laboratory
 * @date May 14, 2016
 * @brief This file contains functions prototype definitions related to Ext-NEC.
 *
 */

#ifndef EXTENDED_NEC_H
#define	EXTENDED_NEC_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include "config.h"

/* Ticks Counter for Extended-NEC Protocol Decoding */  
#define TICK_9MS          128ul     /**< 9ms Counter.*/
#define TICK_8MS          114ul     /**< 8ms Counter.*/
#define TICK_4o5MS        64ul      /**< 4.5ms Counter.*/
#define TICK_4MS          55ul      /**< 4ms Counter.*/
#define TICK_2o5MS        30ul      /**< 2.5ms Counter.*/
#define TICK_3_BURST      23ul      /**< 3 Burst Counter.*/
#define TICK_2_BURST      15ul      /**< 2 Burst Counter.*/
#define TICK_1_BURST      6ul       /**< 1 Burst Counter.*/

#define NEC_INFO_COUNTER  32ul      /**< Information Complete Counter.*/

/* Pin Configuration */
#define IR_PIN_DIR  TRISBbits.TRISB0  /**< IR Signal Reception Pin Direction.*/
#define IR_OUT_PIN  PORTBbits.RB0     /**< IR Signal Reception Pin.*/
  
/**
 * @brief Extended NEC Protocol States.
 *
 * Different States of Extended NEC Protocol used by NEC State Machine.
 */
typedef enum _NEC_State_e
{
	NEC_IDLE = 0,     /**< Default State. */
  NEC_AGC_BURST,    /**< 9ms AGC Burst State. */
  NEC_AGC_SPACE,    /**< 4.5ms AGC Burst State. */
 	NEC_DATA,         /**< Address and Data State. */
  NEC_REPEAT        /**< Last Data Repeat State. */
} NEC_State_e;

/* Function Prototypes for Decoding Extended NEC Protocol */
void NEC_Init( void );
void NEC_State_Machine( void ); // Call this function every 70 usec
boolean NEC_Data_Ready( void );
u16_t Get_NEC_Address( void );
u16_t Get_NEC_Data( void );


#ifdef	__cplusplus
}
#endif

#endif	/* EXTENDED_NEC_H */

