/**
 * @file extended_nec.c
 * @author Embedded Laboratory
 * @date May 14, 2016
 * @brief This file contains functions related to Extended NEC Protocol.
 *
 * This file contains function and variables to related to Extended NEC 
 * Protocol Decoding.
 */

#include "extended_nec.h"

static NEC_State_e nec_state = NEC_IDLE;/**<Track NEC State in StateMachine.*/
static boolean signal_state = LOW;      /**< Track NEC Pin State.*/
static u8_t nec_counter = 0;            /**< NEC Data Counter.*/
static u32_t nec_buffer = 0;            /**< NEC Signal Received Buffer.*/
static u32_t nec_buffer_repeat = 0;     /**< Backup for Repeat Signal.*/
static boolean nec_data_ready = FALSE;  /**< NEC Data is Ready.*/

/**
 * @brief Extended NEC Initialization.
 *
 * The output of TSOP1738 is connected to a micro-controller input/output pin, 
 * this function initializes the data direction register of the pin as input.
 * 
 * Call this function as follow:
 * @code
 * NEC_Init();
 * @endcode
 * @note Call this function, before using any other function of this file.
 */
void NEC_Init( void )
{
  IR_PIN_DIR = 1; // Make Pin As Input Pin
}

/**
 * @brief Extended NEC State Machine.
 *
 * The function automatically decodes the pulses coming from the Extended NEC TV
 * Remote and stores it in internal buffer. This is time critical function, so 
 * make sure to call this function every @ 70usec 
 * (which can be achieved using timer interrupt).
 * 
 * Call this function as follow:
 * @code
 * if( 70us_timer_interrupt_occurs )
 * {
 *  NEC_State_Machine();
 *  Clear_Timer_Flag();
 * }
 * @endcode
 * @note Call this function every 70usec.
 */
void NEC_State_Machine( void )
{
  static u32_t tenusec_counter = 0;
  tenusec_counter++;
  switch(nec_state)
  {
    case NEC_IDLE:
      nec_counter = 0ul;
      if( IR_OUT_PIN == 0 )
      {
        nec_state++;
        tenusec_counter = 0ul;
      }
      break;
    case NEC_AGC_BURST:
      if( IR_OUT_PIN && tenusec_counter <= TICK_8MS )
      {
        nec_state = NEC_IDLE;
      }
      else if( IR_OUT_PIN && tenusec_counter > TICK_8MS )
      {
        nec_state++;
        tenusec_counter = 0ul;
      }
      else
      {
        if( IR_OUT_PIN == 0 && tenusec_counter > TICK_9MS )
        {
          nec_state = NEC_IDLE;
        }
      }
      break;
    case NEC_AGC_SPACE:
      if( !IR_OUT_PIN && tenusec_counter <= TICK_4MS )
      {
        if( tenusec_counter <= TICK_2o5MS )
        {
          // Repeat Pulse
          nec_state = NEC_REPEAT;
        }
        else
        {
          nec_state = NEC_IDLE;
        }
      }
      else if( !IR_OUT_PIN && tenusec_counter > TICK_4MS)
      {
        nec_data_ready = FALSE;
        nec_state++;
        tenusec_counter = 0ul;
        signal_state = HIGH;
      }
      else
      {
        if( IR_OUT_PIN && tenusec_counter > TICK_4o5MS )
        {
          nec_state = NEC_IDLE;
        }
      }
      break;
    case NEC_DATA:
      if( signal_state )
      {
        if( IR_OUT_PIN )
        {
          signal_state = LOW;
          tenusec_counter = 0ul;
        }
      }
      else
      {
        if( !IR_OUT_PIN )
        {
          signal_state = HIGH;
          if(tenusec_counter >= TICK_1_BURST && tenusec_counter < TICK_2_BURST )
          {
            CLR_BIT(nec_buffer,nec_counter);
            nec_counter++;
          }
          else if(tenusec_counter >= TICK_1_BURST && tenusec_counter < TICK_3_BURST )
          {
            SET_BIT(nec_buffer,nec_counter);
            nec_counter++;
          }
          else
            nec_state = NEC_IDLE;

          if(nec_counter >= NEC_INFO_COUNTER )
          {
            nec_buffer_repeat = nec_buffer;
            nec_data_ready = TRUE;
            nec_counter = 0ul;
            nec_state = NEC_IDLE;
          }
        }
      }
      break;
    case NEC_REPEAT:
      nec_buffer = nec_buffer_repeat;
      nec_data_ready = TRUE;
      nec_state = NEC_IDLE;
      break;
    default:
      nec_state = NEC_IDLE;
      break;
  }
}

/**
 * @brief Extended NEC Data Ready.
 *
 * The function returns true if pulses from TV remote are received and decoded.
 * @return TRUE is data is received and decoded, else FALSE.
 * @note This function only tells whether data is received, but doesn't 
 * verify its integrity.
 */
boolean NEC_Data_Ready(void)
{
  boolean data_state = nec_data_ready;
  nec_data_ready = FALSE;
  return data_state;
}

/**
 * @brief Get Decoded Address.
 *
 * The function returns the address of the Remote from which the pulses are 
 * received.
 * @return Address of Remote.
 * @note This function should be used with #NEC_Data_Ready function
 */
u16_t Get_NEC_Address( void )
{
  u32_t temp_address = (nec_buffer & 0x0000FFFF);
  return (u16_t)temp_address;
}

/**
 * @brief Get Decoded Command.
 *
 * The function returns the data received from the Remote, from which the 
 * pulses are received.
 * @return Command from Remote.
 * @note This function should be used with #NEC_Data_Ready function
 * @warning This function return,<b>0xFFFF</b> if data received is not correct.
 */
u16_t Get_NEC_Data( void )
{
  u32_t temp_data = (nec_buffer & 0xFFFF0000);
  u8_t temp_dataLSB, temp_dataMSB;
  temp_data = temp_data>>16;
  // Checking Bits
  temp_dataLSB = (u8_t)temp_data;
  temp_dataMSB = (u8_t)(temp_data>>8);
  temp_dataMSB = ~temp_dataMSB;
  if(temp_dataLSB == temp_dataMSB)
  {
    temp_data = temp_dataLSB;
  }
  else
  {
    temp_data = 0xFF;
  }
  return (u16_t)temp_data;
}
