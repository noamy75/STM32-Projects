/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Noam Yakar
 * @brief          : Main program body
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------- */

#include "main.h"
#include "queue.h"

/* Global variables --------------------------------------------------------- */

/* Allocate tasks objects in memory */
TaskControlBlock_t gIdleTask;
TaskControlBlock_t gTask1;
TaskControlBlock_t gTask2;
TaskControlBlock_t gTask3;
TaskControlBlock_t gTask4;

/* Pointers to the tasks objects */
TaskControlBlock_t *pIdleTask = &gIdleTask;
TaskControlBlock_t *pTask1 = &gTask1;
TaskControlBlock_t *pTask2 = &gTask2;
TaskControlBlock_t *pTask3 = &gTask3;
TaskControlBlock_t *pTask4 = &gTask4;

/* Initialize ready queue and blocked queue */
Queue_t gReadyQueue = {READY_QUEUE, NULL, Enqueue, Dequeue};
Queue_t gBlockedQueue = {BLOCKED_QUEUE, NULL, Enqueue, Dequeue};

/* This variable specifies the current running task */
TaskControlBlock_t *gpCurrentRunningTask = NULL;

/* This variable is the program counter updated by the SysTick handler every 1ms */
uint32_t gTickCount = 0;

/* Functions definitions ---------------------------------------------------- */

/**
  * @brief  Program entry point.
  * @param  None
  * @retval indicates how the program exited.
  */
int main(void)
{
	/* Enable system exceptions */
	System_Exceptions_Enable();

	/* Initialize MSP to the start of the scheduler's stack */
	Scheduler_Stack_Init(SCHEDULER_STACK_START);

	/* Initialize tasks' control block properties and push dummy contents to their stack */
	Task_Init(pTask1, TASK1, (uint32_t*)T1_STACK_START, Task1_Handler);
	Task_Init(pTask2, TASK2, (uint32_t*)T2_STACK_START, Task2_Handler);
	Task_Init(pTask3, TASK3, (uint32_t*)T3_STACK_START, Task3_Handler);
	Task_Init(pTask4, TASK4, (uint32_t*)T4_STACK_START, Task4_Handler);
	Task_Init(pIdleTask, IDLE_TASK, (uint32_t*)IDLE_STACK_START, IdleTask_Handler);

	/* Enqueue tasks to the ready queue */
	gReadyQueue.ENQUEUE(&(gReadyQueue.head), pTask1, REGULAR_ENQUEUE);
	gReadyQueue.ENQUEUE(&(gReadyQueue.head), pTask2, REGULAR_ENQUEUE);
	gReadyQueue.ENQUEUE(&(gReadyQueue.head), pTask3, REGULAR_ENQUEUE);
	gReadyQueue.ENQUEUE(&(gReadyQueue.head), pTask4, REGULAR_ENQUEUE);
	gReadyQueue.ENQUEUE(&(gReadyQueue.head), pIdleTask, REGULAR_ENQUEUE);

	/* Update the current running task */
	Schedule();

	/* Initialize the 4 on-board LEDs */
	Led_Init();

	/* Initialize SysTick to 1KHz */
	SysTick_Init(TICK_HZ);

	/* Set PSP to the current running task's stack pointer and make it the active stack pointer. */
	Switch_SP_To_PSP();

	/* Kick-start with the first task */
	gpCurrentRunningTask->task_handler();

	while(1);
}

/**
  * @brief  Updates the variable current_running_task.
  * @param  None
  * @retval None
  */
void Schedule(void)
{
	TaskControlBlock_t* pScheduledOutTask = NULL;

	/* Only in the beginning there's no running task */
	if(gpCurrentRunningTask == NULL)
	{
		gpCurrentRunningTask = gReadyQueue.DEQUEUE(&(gReadyQueue.head), DEQUEUE_WITH_REAR_IDLE_TASK);
	}

	/* The next task to be scheduled is not the idle task */
	else if(gReadyQueue.head->task_id != IDLE_TASK)
	{
		/* Save the scheduled out task and update the current running task */
		pScheduledOutTask = gpCurrentRunningTask;
		gpCurrentRunningTask = gReadyQueue.DEQUEUE(&(gReadyQueue.head), DEQUEUE_WITH_REAR_IDLE_TASK);

		/* Enqueue the scheduled out task to the ready queue if its not idle and its ready*/
		if(pScheduledOutTask->task_id != IDLE_TASK)
		{
			if(pScheduledOutTask->current_state == TASK_READY_STATE)
			{
				gReadyQueue.ENQUEUE(&(gReadyQueue.head), pScheduledOutTask, ENQUEUE_WITH_REAR_IDLE_TASK);
			}
		}
	}

	/* The next task to be scheduled is the idle task */
	else
	{
		/* Schedule the idle task if no other option is available. A running task that's
		 * in ready state will stay scheduled. */
		if(gpCurrentRunningTask->task_id != IDLE_TASK)
		{
			if(gpCurrentRunningTask->current_state != TASK_READY_STATE)
			{
				pScheduledOutTask = gpCurrentRunningTask;
				gpCurrentRunningTask = gReadyQueue.head; /* Schedule idle task */
			}
		}
	}
}

/**
  * @brief  Stuck in a while(1) loop and does nothing.
  * @param  None
  * @retval None
  */
void IdleTask_Handler(void)
{
	while(1);
}

/**
  * @brief  Toggles the green LED every 1 second.
  * @param  None
  * @retval None
  */
void Task1_Handler(void)
{
	while(1)
	{
		Led_On(LED_GREEN);
		Task_Delay(DELAY_1S);
		Led_Off(LED_GREEN);
		Task_Delay(DELAY_1S);
	}
}

/**
  * @brief  Toggles the orange LED every 500 milliseconds.
  * @param  None
  * @retval None
  */
void Task2_Handler(void)
{
	while(1)
	{
		Led_On(LED_ORANGE);
		Task_Delay(DELAY_500MS);
		Led_Off(LED_ORANGE);
		Task_Delay(DELAY_500MS);
	}
}

/**
  * @brief  Toggles the blue LED every 250 milliseconds.
  * @param  None
  * @retval None
  */
void Task3_Handler(void)
{
	while(1)
	{
		Led_On(LED_BLUE);
		Task_Delay(DELAY_250MS);
		Led_Off(LED_BLUE);
		Task_Delay(DELAY_250MS);
	}
}

/**
  * @brief  Toggles the red LED every 125 milliseconds.
  * @param  None
  * @retval None
  */
void Task4_Handler(void)
{
	while(1)
	{
		Led_On(LED_RED);
		Task_Delay(DELAY_125MS);
		Led_Off(LED_RED);
		Task_Delay(DELAY_125MS);
	}
}

/**
  * @brief  Initialize the processor peripheral SysTick to a certain reload value.
  * @param  TickHz - The wanted ticking frequency in Hz.
  * @retval None
  */
void SysTick_Init(uint32_t TickHz)
{
	/* Define pointers to relevant SysTick registers */
	uint32_t *pSYST_CSR = (uint32_t*)SYST_CSR; /* pointer to SysTick Control and Status Register */
	uint32_t *pSYST_RVR = (uint32_t*)SYST_RVR; /* pointer to SysTick Reload Value Register */

    /* Calculate the reload value */
	uint32_t SystemTicksInOneSecond = (SYSTICK_TIM_CLK/TickHz);
	uint32_t ReloadValue = SystemTicksInOneSecond-1;

	/* Clear RVR and load the reload value */
	*pSYST_RVR &= ~(0x00FFFFFFFF);
	*pSYST_RVR |= ReloadValue;

	/* Enable the SysTick features */
	*pSYST_CSR |= ( 1 << 1); /* Enable SysTick exception request - assert request */
	*pSYST_CSR |= ( 1 << 2); /* Indicates the clock source - processor clock */
	*pSYST_CSR |= ( 1 << 0); /* Enable the counter */
}

/**
  * @brief  Initializes MSP to the start of the scheduler's stack.
  * @param  SchedulerStackStart - Specifies the start of the scheduler's stack.
  * @retval None
  */
__attribute__((naked)) void Scheduler_Stack_Init(uint32_t SchedulerStackStart)
{
	/* Move the value of scheduler_stack_start to MSP using a register */
	__asm volatile("MSR MSP,%0": : "r" (SchedulerStackStart) : );

	/* Branch using LR */
	__asm volatile("BX LR");
}

/**
  * @brief  Initializes a task's control block properties and pushes dummy contents
  * 		to its stack.
  * @note   The stack is Full Descending.
  * @param  pTask - pointer to a TaskControlBlock_t structure that contains task properties.
  * @param  TaskID - a TaskID_e enumerator that specifies the ID of a task.
  * 			This parameter can be one of the following values:
  * 				@arg IDLE_TASK : Idle task
  * 				@arg TASK1 : Task 1
  * 				@arg TASK2 : Task 2
  * 				@arg TASK3 : Task 3
  * 				@arg TASK4 : Task 4
  * @param  pPSPValue - Pointer to the task's stack start that will be used as PSP.
  * @param  pTaskHandler - Pointer to the task handler function.
  * @retval None
  */
void Task_Init(TaskControlBlock_t *pTask, TaskID_e TaskID, uint32_t *pPSPValue, void (*pTaskHandler)(void))
{
	/* Initialize task properties */
	pTask->task_id = TaskID;
	pTask->psp_value = pPSPValue;
	pTask->block_count = 0;
	pTask->current_state = TASK_READY_STATE;
	pTask->task_handler = pTaskHandler;
	pTask->next = NULL;

	/* Get PSP value */
	uint32_t *pPSP = pTask->psp_value;

	/* Push dummy values for core registers xPSR, PC, LR */
	*(--pPSP) = DUMMY_XPSR; /* XPSR = 0x01000000, maintaining T-bit (bit 24) as 1*/
	*(--pPSP) = (uint32_t) pTaskHandler; /* PC */
	*(--pPSP) = EXC_RETURN_THREAD_PSP; /* LR - EXC_RETURN = Return to thread mode and use PSP */

	/* Push zeros for core registers R0-R12*/
	for(int j = 0 ; j < 13 ; j++)
	{
		*(--pPSP) = 0;
	}

	/* Save PSP value */
	pTask->psp_value = pPSP;
}

/**
  * @brief  Enables the MemManage, BusFault and UsageFault system exceptions in the System
  * Handler Control and State Register (SHCSR).
  * @param  None
  * @retval None
  */
void System_Exceptions_Enable(void)
{
	/* Define a pointer to SHCSR */
	uint32_t *pSHCSR = (uint32_t*)SHCRS;

	*pSHCSR |= ( 1 << 16); /* Enable MemManage exception  */
	*pSHCSR |= ( 1 << 17); /* Enable BusFault exception   */
	*pSHCSR |= ( 1 << 18); /* Enable UsageFault exception */
}

/**
  * @brief  Fetches the PSP value of the current running task and returns it.
  * @param  None
  * @retval PSP value of the current running task.
  */
uint32_t Get_PSP_Value(void)
{
	return (uint32_t)(gpCurrentRunningTask->psp_value);
}

/**
  * @brief  Stores the PSP value of the current running task.
  * @param  CurrentPSPValue - The PSP value of the current running task.
  * @retval None
  */
void Save_PSP_Value(uint32_t CurrentPSPValue)
{
	gpCurrentRunningTask->psp_value = (uint32_t*)CurrentPSPValue;
}

/**
  * @brief  Sets the PSP register to the current running task's stack pointer and selects PSP
  * 		as the active stack pointer.
  * @param  None
  * @retval None
  */
__attribute__((naked)) void Switch_SP_To_PSP(void)
{
	/* Set PSP to the current running task's stack pointer */
	__asm volatile ("PUSH {LR}");        /* Before branching, save LR which connects back to main(), on the scheduler stack*/
	__asm volatile ("BL Get_PSP_Value"); /* Get the PSP value of the current running task */
	__asm volatile ("MSR PSP,R0");       /* Set PSP. The value returned from Get_PSP_Value is stored in R0. */
	__asm volatile ("POP {LR}");         /* Retrieve the LR value from the scheduler stack */

	/* Select SP to be PSP using the CONTROL core register */
	__asm volatile ("MOV R0,#0x2");      /* R0 = 0x2 */
	__asm volatile ("MSR CONTROL,R0");   /* CONTROL[1] = SPSEL = 1 */

	/* Branch back to main() */
	__asm volatile ("BX LR");
}

/**
  * @brief  Changes the PendSV exception state to pending.
  * @param  None
  * @retval None
  */
void Pend_PendSV(void)
{
	/* Define a pointer to ICSR, a System Control Block register */
	uint32_t *pICSR = (uint32_t*)ICSR; /* ICSR - Interrupt Control and State Register */

	/* Change the PendSV exception state to pending */
	*pICSR |= ( 1 << 28);
}

/**
  * @brief  Puts the current running task in BLOCKED state and initiates a contect-switch (Task Yield).
  * @param  DelayTickCount - Specifies the duration in terms of SysTick ticks the task should be blocked.
  * @retval None
  */
void Task_Delay(uint32_t DelayTickCount)
{
	/* Disable interrupts */
	INTERRUPT_DISABLE();

	/* Delay is relevant only for LED tasks */
	if(gpCurrentRunningTask->task_id != IDLE_TASK)
	{
		/* Set the task's block count to tick_count ticks from now*/
		gpCurrentRunningTask->block_count = gTickCount + DelayTickCount;

		/* Change task state to BLOCKED */
		gpCurrentRunningTask->current_state = TASK_BLOCKED_STATE;

		/* Insert the blocked task to the blocked queue, and keep it sorted */
		gBlockedQueue.ENQUEUE(&(gBlockedQueue.head), gpCurrentRunningTask, ENQUEUE_SORTED);

		/* Pend the PendSV exception and initiate a contect-switch */
		Pend_PendSV();
	}

	/* Enable interrupts */
	INTERRUPT_ENABLE();
}

/**
  * @brief  Increments the global tick count variable g_tick_count.
  * @param  None
  * @retval None
  */
void Increment_Global_Tick_Count(void)
{
	gTickCount++;
}

/**
  * @brief  Checks the blocked queue and puts qualified tasks in READY state.
  * @param  None
  * @retval None
  */
void Unblock_Tasks(void)
{
	/* It's enough to check the head in each iteration, since the queue is sorted and qualified tasks
	 * get unblocked immediately */
	while(gBlockedQueue.head != NULL)
	{
		if(gBlockedQueue.head->block_count == gTickCount)
		{

			/* Dequeue the qualified task */
			TaskControlBlock_t* temp = gBlockedQueue.DEQUEUE(&(gBlockedQueue.head), REGULAR_DEQUEUE);

			/* Change task state to READY */
			temp->current_state = TASK_READY_STATE;

			/* Insert the ready task to the ready queue */
			gReadyQueue.ENQUEUE(&(gReadyQueue.head), temp, ENQUEUE_WITH_REAR_IDLE_TASK);
		}

		/* Since the queue is sorted, once reaching a head task that's not qualified for unblocking,
		 * it's sure that all other tasks aren't qualified as well */
		else
		{
			break;
		}
	}
}
