/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              G U A R D I A N                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* The system's central interrupt handling routine.                          */
/* The parameter specifies the number of the interrupt that occurred.         */
/*****************************************************************************/

/* INCLUDES */
#include "machine/plugbox.h"
#include "guard/guard.h"

/*
 * The Plugbox maps interrupt slots to their handler objects.
 *
 * Example:
 *   keyboard interrupt slot -> Keyboard object
 */
extern Plugbox plugbox;

/*
 * Global Guard object.
 *
 * guardian() needs the Guard so it can pass interrupt epilogues to it.
 * The Guard then decides whether the epilogue runs immediately or is queued.
 */
extern Guard guard;

/* FUNCTIONS */
extern "C" void guardian(unsigned int slot);

/*
 * GUARDIAN
 * --------
 * Central interrupt handling function.
 *
 * Flow:
 *
 *   1. An interrupt happens.
 *   2. Low-level interrupt code calls guardian(slot).
 *   3. guardian() asks the Plugbox which Gate belongs to this slot.
 *   4. guardian() calls trigger(), the prologue method.
 *   5. If trigger() returns true, the epilogue should run.
 *   6. guardian() passes the Gate to Guard::relay().
 *
 * Important:
 *   trigger() does not run the full interrupt work.
 *   trigger() only performs the short prologue decision.
 *
 *   relay() decides whether epilogue() runs now or later.
 */
void guardian(unsigned int slot)
{
    /*
     * Get the interrupt handler object for this interrupt slot.
     *
     * item is a reference to a Gate, for example the Keyboard object.
     */
    Gate& item = plugbox.report(slot);

    /*
     * Run the prologue.
     *
     * If it returns true, this interrupt has epilogue work to do.
     */
    if (item.trigger()) {
        /*
         * Give the Gate to the Guard.
         *
         * Guard will either:
         * - run item.epilogue() immediately, or
         * - queue the item for later
         */
        guard.relay(&item);
    }
}