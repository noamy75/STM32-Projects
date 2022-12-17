/**
 ******************************************************************************
 * @file           : queue.c
 * @author         : Noam Yakar
 * @brief          : This file contains function definitions for queue operations.
 ******************************************************************************
 */

/* Includes ----------------------------------------------------------------- */

#include "queue.h"

/* Functions definitions ---------------------------------------------------- */

/**
  * @brief  Enqueues a task to a queue in a certain mode.
  * @param  pHead - Pointer to a pointer to the first element of the queue.
  * @param  pTask - Pointer to the task to be enqueued.
  * @param  EnqueueMode - an EnqueueMode_e enumerator that specifies the method in which a task is
  *         inserted to a queue.
  *             This parameter can be one of the following values:
  *                 @arg REGULAR_ENQUEUE : Insert at the end of the queue.
  *                 @arg ENQUEUE_WITH_REAR_IDLE_TASK : Insert one item before the end of the queue.
  *                                                    At the end there's the idle task.
  *                 @arg ENQUEUE_SORTED : The queue is actually a sorted linked list, sorted by the
	                                      block_count property, and the new task is placed according
	                                      to its block_count value
  * @retval None
  */
void Enqueue(TaskControlBlock_t **pHead, TaskControlBlock_t *pTask, EnqueueMode_e EnqueueMode)
{
	/* Enqueue a task while maintaining the idle task at the back */
	if(EnqueueMode == ENQUEUE_WITH_REAR_IDLE_TASK)
	{
		/* The queue is empty, the new task becomes the queue head */
		if (*pHead == NULL)
		{
			*pHead = pTask;
			pTask->next = NULL;
		}

		/* The queue has only one element which is the idle task, the new task becomes the queue head */
		else if((*pHead)->task_id == IDLE_TASK)
		{
			pTask->next = *pHead;
			*pHead = pTask;
		}

		/* The queue contains led tasks, insert the new task at the end but before the idle task */
		else
		{
			/* Iterate through the tasks until reaching the one before the idle task */
			TaskControlBlock_t* iter = *pHead;
			while (iter->next->task_id != IDLE_TASK)
			{
				iter = iter->next;
			}

			/* Insert the new task between the last led task and the idle task */
			TaskControlBlock_t* temp = iter->next;
			iter->next = pTask;
			pTask->next = temp;
		}
	}

	/* Enqueue a task regularly, at the back of the queue */
	else if(EnqueueMode == REGULAR_ENQUEUE)
	{
		/* The queue is empty, the new task becomes the queue head */
		if (*pHead == NULL)
		{
			*pHead = pTask;
			pTask->next = NULL;
		}

		/* The queue contains led tasks, insert the new task at the end */
		else
		{
			/* Iterate through the tasks until reaching the last one */
			TaskControlBlock_t* iter = *pHead;
			while (iter->next != NULL)
			{
				iter = iter->next;
			}

			/* Insert the new task at the end */
			iter->next = pTask;
			pTask->next = NULL;
		}
	}

	/* Enqueue a task while keeping it sorted by the block_count property. In this case the queue
	 * is actually a sorted linked list */
	else /* if(EnqueueMode == ENQUEUE_SORTED) */
	{
		/* The queue is empty, the new task becomes the queue head */
		if (*pHead == NULL)
		{
			*pHead = pTask;
			pTask->next = NULL;
		}

		/* The new task's block_count is smaller than the first element's, the new task becomes the queue head */
		else if((*pHead)->block_count > pTask->block_count)
		{
			pTask->next = *pHead;
			*pHead = pTask;
		}

		/* The new task should be inserted in the middle of the queue */
		else
		{
			TaskControlBlock_t* prev = *pHead;
			TaskControlBlock_t* iter = (*pHead)->next;
			while(iter != NULL)
			{
				/* The new task has bigger block_count than the current task, keep searching */
				if(iter->block_count < pTask->block_count)
				{
					prev = iter;
					iter = iter->next;
				}

				/* The new task has smaller block_count than the current task, insert the new task */
				else
				{
					prev->next = pTask;
					pTask->next = iter;
					break;
				}
			}

			/* Reached end of the queue, insert the new task at end */
			if(iter == NULL)
			{
				prev->next = pTask;
				pTask->next = NULL;
			}
		}
	}
}

/**
  * @brief  Dequeues a task from a queue in a certain mode.
  * @param  pHead - Pointer to a pointer to the first element of the queue.
  * @param  DequeueMode - a DequeueMode_e enumerator that specifies the method in which a task is
  *         removed from a queue.
  *             This parameter can be one of the following values:
  *                 @arg REGULAR_DEQUEUE : Insert at the end of the queue.
  *                 @arg DEQUEUE_WITH_REAR_IDLE_TASK : The task is dequeued from the beginning of the
  *                                                    queue, but the idle task is not dequeued.
  * @retval None
  */
TaskControlBlock_t* Dequeue(TaskControlBlock_t **pHead, DequeueMode_e DequeueMode)
{
	if(DequeueMode == DEQUEUE_WITH_REAR_IDLE_TASK)
	{
		if (*pHead == NULL)
		{
			printf("Error: trying to dequeue from an empty list");
			return NULL;
		}
		else if((*pHead)->task_id == IDLE_TASK)
		{
			printf("Error: trying to dequeue idle task from the ready list");
			return NULL;
		}
		else
		{
			TaskControlBlock_t* temp = *pHead;
			*pHead = (*pHead)->next;
			return temp;
		}
	}

	else //if(dequeue_mode == REGULAR_DEQUEUE)
	{
		if (*pHead == NULL)
		{
			printf("Error: trying to dequeue from an empty list");
			return NULL;
		}
		else
		{
			TaskControlBlock_t* temp = *pHead;
			*pHead = (*pHead)->next;
			return temp;
		}
	}
}
