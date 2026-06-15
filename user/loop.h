/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 L O O P                                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Loop is a thread that does nothing else but count upwards and print this  */
/* on the screen. In between, it yields the CPU. The Scheduler then decides  */
/* which thread shall run next.                                              */
/*****************************************************************************/

#ifndef __loop_include__
#define __loop_include__

/* Add your code here */ 
#include "guard/secure.h"
#include "thread/entrant.h"
#include "thread/scheduler.h"
#include "device/cgastr.h"

class Loop : public Entrant
 
{

public:
	Loop (const Loop &copy) = delete; // prevent copying
	Loop& operator=(const Loop&) = delete; // prevent assignment
/* Add your code here */ 
	Loop(void* tos) : Entrant(tos){}
	void action () override;
};
 
#endif
