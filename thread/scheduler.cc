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
/* Add your code here */ 
