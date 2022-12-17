/**
 ******************************************************************************
 * @file           : led.c
 * @author         : Noam Yakar
 * @brief          : This file contains function definitions for LEDs operations.
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------- */

#include "led.h"

/* Functions definitions ---------------------------------------------------- */

/**
  * @brief  Initialize the 4 on-board LEDs.
  * @note   The 4 LEDs are connected to port D as pins 12,13,14,15.
  * @param  None
  * @retval None
  */
void Led_Init(void)
{
	/* Define pointers */
	uint32_t *pRccAhb1enr = (uint32_t*)RCC_AHB1ENR; /* pointer to RCC AHB1 peripheral clock enable register */
	uint32_t *pGpiodModeReg = (uint32_t*)GPIOD_MODER; /* pointer to GPIOD port mode register */

	/* Enable  the peripheral clock of GPIOD */
	*pRccAhb1enr |= ( 1 << 3);

	/* Configure general purpose output mode for pins 12,13,14,15 of port D */
	*pGpiodModeReg |= ( 1 << (2 * LED_GREEN));  /* pGpiodModeReg[25:24] = 01 */
	*pGpiodModeReg |= ( 1 << (2 * LED_ORANGE)); /* pGpiodModeReg[27:26] = 01 */
	*pGpiodModeReg |= ( 1 << (2 * LED_RED));    /* pGpiodModeReg[29:28] = 01 */
	*pGpiodModeReg |= ( 1 << (2 * LED_BLUE));   /* pGpiodModeReg[31:30] = 01 */

	/* Turn off all LEDs */
	Led_Off(LED_GREEN);
	Led_Off(LED_ORANGE);
	Led_Off(LED_RED);
	Led_Off(LED_BLUE);
}

/**
  * @brief  Turn on a LED.
  * @param  LedNumber - Specifies the pin to which the led is connected to.
  * @retval None
  */
void Led_On(uint8_t LedNumber)
{
	uint32_t *pGpiodDataReg = (uint32_t*)GPIOD_ODR;
	*pGpiodDataReg |= (1 << LedNumber); /* pGpiodDataReg[LedNumber] = 1 */
}

/**
  * @brief  Turn off a LED.
  * @param  LedNumber - Specifies the pin to which the led is connected to.
  * @retval None
  */
void Led_Off(uint8_t LedNumber)
{
	uint32_t *pGpiodDataReg = (uint32_t*)GPIOD_ODR;
	*pGpiodDataReg &= ~(1 << LedNumber); /* pGpiodDataReg[LedNumber] = 0 */
}
