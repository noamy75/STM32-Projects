/**
 ******************************************************************************
 * @file           : led.h
 * @author         : Noam Yakar
 * @brief          : Header file of Led module. This file contains macros and
 *					 functions prototypes.
 ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef LED_H_
#define LED_H_

/* Includes ----------------------------------------------------------------- */

#include <stdint.h>

/* Macros ------------------------------------------------------------------- */

/* RCC address calculations */
#define RCC_AHB1_BASE 0x40023800
#define RCC_AHB1ENR_OFFSET 0x30
#define RCC_AHB1ENR ( (RCC_AHB1_BASE) + (RCC_AHB1ENR_OFFSET) )

/* GPIOD address calculations */
#define GPIOD_BASE 0x40020C00
#define GPIOx_MODER_OFFSET 0x00
#define GPIOD_MODER ( (GPIOD_BASE) + (GPIOx_MODER_OFFSET) )
#define GPIOx_ODR_OFFSET 0x14
#define GPIOD_ODR ( (GPIOD_BASE) + (GPIOx_ODR_OFFSET) )

/* LEDs pin numbers */
#define LED_GREEN  12
#define LED_ORANGE 13
#define LED_RED    14
#define LED_BLUE   15

/* Time delays */
#define DELAY_1S  		(1000U)
#define DELAY_500MS  	(500U)
#define DELAY_250MS 	(250U)
#define DELAY_125MS 	(125U)

/* Functions prototypes ------------------------------------------------------ */

void Led_Init(void);
void Led_On(uint8_t LedNumber);
void Led_Off(uint8_t LedNumber);

#endif /* LED_H_ */
