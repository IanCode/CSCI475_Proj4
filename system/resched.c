/* resched.c - resched */

#include <xinu.h>

int resched_count;	/* Tracks how many times reschedule is called */

/**
 * Reschedule processor to next ready process
 *
 */
void	resched(void)		// assumes interrupts are disabled
{
	//Iterate resched_count
	resched_count++;

	//run deadlock detection every 50 times reschedule() is called
	if(resched_count == 50)
	{		
		//disable interrupts
		intmask mask = disable();

		//look for deadlocks
		deadlock_detect();

		//restore interrupts
		restore(mask);
	
		//reset the count
		resched_count = 0;
	}
	
	struct procent *ptold;	// ptr to table entry for old process
	struct procent *ptnew;	// ptr to table entry for new process

	// If rescheduling is deferred, record attempt and return
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	//implement aging policy
	if(AGING == TRUE)	
	{
		if(nonempty(readyqueue) == TRUE)
		{
			//do not increase the priority of the node to be scheduled for running
			struct qentry *node = readyqueue->head;
			while(node != NULL)
			{
				//if not the null process
				if(node->pid != 0)
				{
					//increment the key (priority) of every process on the queue
					INCREMENT(node->key);
				}

				//increment node
				node = node->next;
			}
		}
	}

	// Point to process table entry for the current (old) process
	ptold = &proctab[currpid];

	// TODO - check ptold's state. If it's running, put it on the ready queue and change state to ready
	if(ptold->prstate == PR_CURR) {

		currpid = enqueue(currpid, readyqueue, ptold->prprio);
		ptold->prstate = PR_READY;
	}

	// TODO - dequeue next process off the ready queue and point ptnew to it
	pid32 xpid = dequeue(readyqueue);
	ptnew = &proctab[xpid];

	// TODO - change its state to "current" (i.e., running)
	ptnew->prstate = PR_CURR;

	//reset preempt
	preempt = QUANTUM;

	// TODO - set currpid to reflect new running process' PID
	currpid = xpid;

	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	// Old process returns here when resumed
	return;
}
