/**
 ******************************************************************************
 * @file           : main.h
 * @author         : Noam Yakar
 * @brief          : Header file of Main module. This file contains enumerations,
 * 					 macros, structures definitions and functions prototypes.
 ******************************************************************************
*/

/* Define to prevent recursive inclusion ---------------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

/* Includes ------------------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include "led.h"

/* Macros --------------------------------------------------------------- */

/* SysTick registers */
#define SYST_CSR                 0xE000E010
#define SYST_RVR                 0xE000E014

/* System Control Block registers */
#define ICSR                     0xE000ED04
#define SHCRS                    0xE000ED24

/* Stack sizes */
#define SIZE_TASK_STACK          1024U
#define SIZE_SCHEDULER_STACK     1024U

/* SRAM boundaries */
#define SRAM_START               0x20000000U
#define SIZE_SRAM                ( (128) * (1024))
#define SRAM_END                 ((SRAM_START) + (SIZE_SRAM) )

/* Stack boundaries */
#define T1_STACK_START           SRAM_END
#define T2_STACK_START           ( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START           ( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START           ( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define IDLE_STACK_START         ( (SRAM_END) - (4 * SIZE_TASK_STACK) )
#define SCHEDULER_STACK_START    ( (SRAM_END) - (5 * SIZE_TASK_STACK) )

/* Clocking */
#define TICK_HZ                  1000U
#define HSI_CLOCK                16000000U
#define SYSTICK_TIM_CLK          HSI_CLOCK

/* Dummy value for xPSR register */
#define DUMMY_XPSR               0x01000000U       /* Maintain T-bit (bit 24) as 1 */

/* The following EXC_RETURN values are saved the LR on exception entry */
#define EXC_RETURN_HANDLER       (0xFFFFFFF1UL)    /* return to Handler mode, use MSP after return */
#define EXC_RETURN_THREAD_MSP    (0xFFFFFFF9UL)    /* return to Thread mode, use MSP after return  */
#define EXC_RETURN_THREAD_PSP    (0xFFFFFFFDUL)    /* return to Thread mode, use PSP after return  */

/* Interrupts Enable/Disable */
#define INTERRUPT_DISABLE()  do{__asm volatile ("MOV R0,#0x1"); asm volatile("MSR PRIMASK,R0"); } while(0)
#define INTERRUPT_ENABLE()   do{__asm volatile ("MOV R0,#0x0"); asm volatile("MSR PRIMASK,R0"); } while(0)

/* Types --------------------------------------------------------------- */

/* Task IDs */
typedef enum TaskID
{
	IDLE_TASK,
	TASK1,
	TASK2,
	TASK3,
	TASK4
} TaskID_e;

/* Task states */
typedef enum TaskState
{
	TASK_READY_STATE,
	TASK_BLOCKED_STATE,
	TASK_TERMINATED_STATE
} TaskState_e;

/* Task Control Block (TCB) structure definition. Contains private information of a task. */
typedef struct TCB
{
	TaskID_e task_id;               /*!< Specifies the task's ID. This parameter can be any value of @ref TaskID_e */
	uint32_t *psp_value;            /*!< Specifies the task's private stack pointer. */
	uint32_t block_count;           /*!< Specifies the task's block duration if it's in BLOCKED state */
	TaskState_e current_state;      /*!< Specifies the task's state. This parameter can be any value of @ref TaskState_e */
	void (*task_handler)(void);     /*!< Pointer to the task's handler function. */
	struct TCB *next;               /*!< Pointer to the next task's TCB in a queue */
} TaskControlBlock_t;

/* Functions prototypes --------------------------------------------------------- */

void IdleTask_Handler(void);
void Task1_Handler(void);
void Task2_Handler(void);
void Task3_Handler(void);
void Task4_Handler(void);
void Schedule(void);
void SysTick_Init(uint32_t TickHz);
__attribute__((naked)) void Scheduler_Stack_Init(uint32_t SchedulerStackStart);
void Task_Init(TaskControlBlock_t *pTask, TaskID_e TaskID, uint32_t *pPSPValue, void (*pTaskHandler)(void));
void System_Exceptions_Enable(void);
uint32_t Get_PSP_Value(void);
void Save_PSP_Value(uint32_t CurrentPSPValue);
__attribute__((naked)) void Switch_SP_To_PSP(void);
void Pend_PendSV(void);
void Task_Delay(uint32_t DelayTickCount);
void Increment_Global_Tick_Count(void);
void Unblock_Tasks(void);

#endif /* MAIN_H_ */
