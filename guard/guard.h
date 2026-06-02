/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  G U A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Synchronization object for critical sections and interrupt epilogues.      */
/*****************************************************************************/

#ifndef __Guard_include__
#define __Guard_include__

#include "guard/locker.h"
#include "object/queue.h"
#include "guard/gate.h"

/*
 * Guard combines two responsibilities:
 *
 * 1. It behaves like a Locker.
 *    That means it knows whether a critical section is currently free
 *    or occupied.
 *
 * 2. It manages delayed interrupt work.
 *    If an interrupt epilogue cannot run immediately, the corresponding
 *    Gate object is stored in the queue.
 */
class Guard : public Locker {
private:
    /*
     * Queue of Gate objects waiting for their epilogue() method.
     *
     * The queue stores Chain objects. Gate inherits from Chain, so Gate
     * objects can be inserted here.
     */
    Queue queue;

public:
    /*
     * Called when regular control flow leaves a critical section.
     *
     * After leaving, accumulated epilogues may be processed.
     */
    void leave();

    /*
     * Called when an interrupt prologue says:
     *
     *   "My epilogue should run."
     *
     * If the critical section is free, the epilogue can run immediately.
     * If the critical section is occupied, the Gate is queued for later.
     */
    void relay(Gate* item);
};

#endif