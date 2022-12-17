/**
 ******************************************************************************
 * @file           : it.c
 * @author         : Noam Yakar
 * @brief          : This file contains function definitions of handlers for
 * 					 system exceptions and interrupts.
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------- */

#include "it.h"

/* Functions definitions ---------------------------------------------------- */

/**
  * @brief  Handler for the PendSV system exception. Manages the context switching of tasks:
  * 		1.  Saves the values of R4-R11 registers (SF2) of the switched out task, that were
  * 			not part of the standard stack frame during the stacking process that took place
  * 			at the exception entry.
  * 		2.  calls Schedule()
  * 		1.  Retrieves the values of R4-R11 registers (SF2) of the switched in task, that were
  * 			not part of the standard stack frame during the un-stacking process that took place
  * 			at the exception entry.
  * @param  None
  * @retval None
  */
__attribute__((naked)) void PendSV_Handler(void)
{
	/* Save the context of current running task */

	__asm volatile("MRS R0,PSP"); /* Get current running task's PSP value */

	__asm volatile("STMDB R0!,{R4-R11}"); /* Using that PSP value store SF2 (registers R4-R11) */

	__asm volatile("PUSH {LR}"); /* Save LR (EXC_RETURN) on the scheduler main stack */

	__asm volatile("BL Save_PSP_Value"); /* Save the PSP value in the current task's TCB. The PSP value
                                            is in R0 and passed as the first argument to the called function */

	/* Retrieve the context of the next task */

	__asm volatile("BL Schedule"); /* Decide the next task to run */

	__asm volatile ("BL Get_PSP_Value"); /* Get the new task's PSP value */

	__asm volatile ("LDMIA R0!,{R4-R11}"); /* Using that PSP value retrieve SF2 (registers R4-R11) */

	__asm volatile("MSR PSP,R0"); /* Update PSP */

	__asm volatile("POP {LR}"); /* Retrieve LR (EXC_RETURN) from the scheduler main stack*/

	__asm volatile("BX LR"); /* Exception return using the EXC_RETURN in LR*/
}

/**
  * @brief  Handler for the SysTick system exception. Takes place every 1ms. It increments the
  * 		program's global tick count variable - g_tick_count, unblocks qualified tasks and
  * 		initiates a contect-switch.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	/* Increment the program's global tick count */
	Increment_Global_Tick_Count();

	/* Unblock qualified tasks */
	Unblock_Tasks();

	/* Pend the PendSV exception and initiate a contect-switch */
	Pend_PendSV();
}

/**
  * @brief  Handler for the HardFault system exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("Exception : HardFault\n");
	while(1);
}

/**
  * @brief  Handler for the MemManage system exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

/**
  * @brief  Handler for the BusFault system exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}

/**
  * @brief  Handler for the UsageFault system exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	printf("Exception : UsageFault\n");
	while(1);
}
