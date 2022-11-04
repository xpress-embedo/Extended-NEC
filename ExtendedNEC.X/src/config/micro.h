/**
 * @file micro.h
 * @author Embedded Laboratory
 * @date April 29, 2016
 * @brief Project related Data Types and other Macros.
 *
 */

#ifndef MICRO_H
#define	MICRO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define	NULL	0u          /**< NULL Character.*/

// Data Types
typedef uint8_t  u8_t;     /**< 8 bits unsigned. */
typedef uint16_t u16_t;    /**< 16 bits unsigned. */
typedef uint32_t u32_t;    /**< 32 bits unsigned. */
typedef int8_t   s8_t;     /**< 8 bits signed. */
typedef int16_t  s16_t;    /**< 16 bits signed. */
typedef int32_t  s32_t;    /**< 32 bits signed. */

typedef uint8_t boolean;  /**< boolean is 8 bits unsigned.*/

// Definitions

#ifndef FALSE
#define FALSE         0U    /**< FALSE is Equivalent to 0u.*/
#endif
#ifndef TRUE
#define TRUE          1U    /**< TRUE is Equivalent to 1u.*/
#endif

#ifndef LOW
#define LOW           0U    /**< LOW is Equivalent to 0u.*/
#endif
#ifndef HIGH
#define HIGH          1U    /**< HIGH is Equivalent to 1u.*/
#endif

#define SET_BIT(p,n)  ((p) |= ((1ul) << (n)))   /**<Set the bit of variable.*/
#define CLR_BIT(p,n)  ((p) &= ~((1ul) << (n)))  /**<Clear the bit of variable.*/


#ifdef	__cplusplus
}
#endif

#endif	/* MICRO_H */

