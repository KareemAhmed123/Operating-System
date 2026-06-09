/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          D I S P A T C H E R                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Dispatcher implementation.                                                */
/* The Dispatcher maintains the life pointer that points to the currently    */
/* active coroutine. go() initializes the life pointer and starts the first  */
/* coroutine, all further context switches are triggered by dispatch().      */
/* active() returns the life pointer.                                        */
/*****************************************************************************/


#include "thread/dispatch.h"
#include "thread/coroutine.h"

Dispatcher::Dispatcher()
	: life(nullptr)
{}

void Dispatcher::go(Coroutine& first)
{
	// Record the first coroutine before transferring control to it.
	life = &first;
	first.go();
}

void Dispatcher::dispatch(Coroutine& next)
{
	// Keep the old coroutine so its context can be saved by resume().
	Coroutine* previous = life;

	// Update life first: after the switch, next is the active coroutine.
	life = &next;
	previous->resume(next);
}

Coroutine* Dispatcher::active()
{
	return life;
}
