/**
 ******************************************************************************
 * @file           : queue.h
 * @author         : Noam Yakar
 * @brief          : Header file of Queue module. This file contains enumerations,
 * 					 macros, structures definitions and functions prototypes.
 ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef QUEUE_H_
#define QUEUE_H_

/* Includes ----------------------------------------------------------------- */

#include "main.h"

/* Types -------------------------------------------------------------------- */

/* Type of a queue */
typedef enum
{
	READY_QUEUE,
	BLOCKED_QUEUE
} QueueType_e;

/* The method in which a task is inserted to a queue */
typedef enum
{
	REGULAR_ENQUEUE,               /*!< The new task is inserted at the end of the queue */
	ENQUEUE_WITH_REAR_IDLE_TASK,   /*!< The new task is inserted one item before the end of
										the queue. At the end there's the idle task */
	ENQUEUE_SORTED				   /*!< The queue is actually a sorted linked list, sorted by the
										block_count property, and the new task is placed according
										to its block_count value */
} EnqueueMode_e;

/* The method in which a task is dequeued from a queue */
typedef enum
{
	REGULAR_DEQUEUE,				/*!< The task is dequeued from the beginning of the queue */
	DEQUEUE_WITH_REAR_IDLE_TASK,	/*!< The task is dequeued from the beginning of the queue,
	 	 	 	 	 	 	 	 	 	 but the idle task is not dequeued. */
} DequeueMode_e;

/* Pointers to functions */
typedef void (*enqueue)(TaskControlBlock_t**, TaskControlBlock_t*, EnqueueMode_e);
typedef TaskControlBlock_t* (*dequeue)(TaskControlBlock_t**, DequeueMode_e);

/* Queue structure definition. */
typedef struct
{
	QueueType_e queue_type;			/*!< Specifies the queue's type. This parameter can be any value of @ref QueueType_e */
	TaskControlBlock_t* head;		/*!< Pointer to the first element of the queue. */
	enqueue ENQUEUE;				/*!< Pointer to the enqueue function. */
	dequeue DEQUEUE;				/*!< Pointer to the dequeue function. */
} Queue_t;

/* Functions prototypes ------------------------------------------------------ */

void Enqueue(TaskControlBlock_t **pHead, TaskControlBlock_t *pTask, EnqueueMode_e EnqueueMode);
TaskControlBlock_t* Dequeue(TaskControlBlock_t **pHead, DequeueMode_e DequeueMode);

#endif /* QUEUE_H_ */
