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

#ifndef __dispatch_include__
#define __dispatch_include__


class Coroutine;

/**
 * Manages the coroutine that currently owns the processor.
 *
 * Dispatcher does not create or destroy coroutines. It only stores a pointer
 * to the active coroutine and delegates the actual context switch to the
 * Coroutine class.
 */
class Dispatcher {
private:
	/** Coroutine that currently owns the processor, or nullptr before go(). */
	Coroutine* life;

public:
	/** Creates a dispatcher without an active coroutine. */
	Dispatcher();

	Dispatcher(const Dispatcher &copy) = delete; // prevent copying
	Dispatcher& operator=(const Dispatcher&) = delete; // prevent assignment

	/**
	 * Starts the first coroutine.
	 *
	 * @param first Coroutine that initially receives control of the processor.
	 */
	void go(Coroutine& first);

	/**
	 * Switches from the active coroutine to next.
	 *
	 * @param next Coroutine that shall receive control of the processor.
	 */
	void dispatch(Coroutine& next);

	/**
	 * Returns the coroutine currently known as active.
	 *
	 * @return Active coroutine, or nullptr if go() has not been called.
	 */
	Coroutine* active();
};

#endif
