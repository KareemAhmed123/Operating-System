/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                  G A T E                                  */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Class of objects that handle interrupts.                                  */
/*****************************************************************************/

#ifndef __Gate_include__
#define __Gate_include__

#include "object/chain.h"

/*
 * Gate is the base class for interrupt handler objects.
 *
 * It inherits from Chain so Gate objects can be inserted into the
 * epilogue Queue used by the Guard.
 */
class Gate : public Chain {
private:
    bool is_queued;

public:
    Gate()
    {
        next = 0;
        is_queued = false;
    }

    /*
     * Prologue method.
     *
     * Returns true if the epilogue should run later.
     * Returns false if no epilogue is needed.
     */
    virtual bool prologue() = 0;

    /*
     * Epilogue method.
     *
     * This contains the delayed interrupt work that should run later
     * under the control of the Guard.
     */
    virtual void epilogue()
    {
    }

    void queued(bool q)
    {
        is_queued = q;
    }

    bool queued()
    {
        return is_queued;
    }
};

#endif
