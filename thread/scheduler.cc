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
    go(*next);
}

void Scheduler :: exit(){
	Entrant* next = (Entrant*) ready_list.dequeue();
    dispatch(*next);
}

void Scheduler :: kill(Entrant& that){
	if (active() == &that){
		exit();
	} else {
		ready_list.remove(&that); 
		Entrant* next = (Entrant*) ready_list.dequeue();
		dispatch(*next);
	}
}

void Scheduler :: resume(){
	Entrant *current = (Entrant*) active();
    ready(*current);
	Entrant* next = (Entrant*) ready_list.dequeue();
	dispatch(*next);
}
/* Add your code here */ 
