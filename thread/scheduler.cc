/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          S C H E D U L E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Scheduler implementation.                                                 */
/*****************************************************************************/

/* Add your code here */ 
 #include "thread/scheduler.h"
/* Add your code here */ 

void Scheduler::ready (Entrant& that){
	ready_list.enqueue(&that);
}

void Scheduler :: schedule(){
	Entrant* next = (Entrant*) ready_list.dequeue();
    dispatch(*next);
}

void Scheduler :: exit(){
	schedule();
}

void Scheduler :: kill(Entrant& that){
	ready_list.remove(&that);
}

void Scheduler :: resume(){
	Entrant *current = (Entrant*) active();
    ready(*current);
	Entrant* next = (Entrant*) ready_list.dequeue();
	dispatch(*next);
}
/* Add your code here */ 
