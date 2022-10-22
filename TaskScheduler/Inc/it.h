/**
 ******************************************************************************
 * @file           : it.h
 * @author         : Noam Yakar
 * @brief          : Header file of It module. This file contains functions
 * 					 prototypes.
 ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef IT_H_
#define IT_H_

/* Includes ----------------------------------------------------------------- */

#include "main.h"

/* Functions prototypes ----------------------------------------------------- */

__attribute__((naked)) void PendSV_Handler(void);
void SysTick_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#endif /* IT_H_ */
