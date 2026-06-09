/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            E N T R A N T                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* A coroutine that is managed by the Scheduler.                             */
/*****************************************************************************/

#ifndef __entrant_include__
#define __entrant_include__

/* Add your code here */ 
#include "thread/coroutine.h"
#include "object/chain.h"

class Entrant : public Chain, public Coroutine
/* Add your code here */ 
{
public:
	Entrant(const Entrant &copy) = delete; // prevent copying
	Entrant& operator=(const Entrant&) = delete; // prevent assignment

/* Add your code here */ 
	Entrant (void* tos): Coroutine(tos){}
};

#endif
