/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Scheduler implementation.                                                 */
/*****************************************************************************/

#ifndef __schedule_include__
#define __schedule_include__

#include "thread/dispatch.h"
/* Add your code here */ 
#include "thread/entrant.h"
#include "object/queue.h"
class Scheduler : public Dispatcher
/* Add your code here */ 
{
private:
	Queue ready_list;
public:
	Scheduler (const Scheduler &copy) = delete; // prevent copying
	Scheduler& operator=(const Scheduler&) = delete; // prevent assignment
/* Add your code here */ 
	Scheduler() {}

	void ready (Entrant& that);//registers the process that with the scheduler, appended to end of ready list.
	void schedule ();//tarts up scheduling by removing the first process from the ready list and activating it.
	void exit();//With this method a process can terminate itself.
	void kill (Entrant& that); // kill´s that thread
	void resume();//trigger a context switch without the calling Entrant having to know which other Entrant
 
};

#endif
