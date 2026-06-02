#include "guard/guard.h"
#include "machine/cpu.h"

/*
 * We use the global CPU object only for enabling interrupts again.
 *
 * Important context:
 * When a hardware interrupt happens, the low-level interrupt entry code
 * disables interrupts before it calls guardian().
 *
 * That means:
 *
 *   interrupt happens
 *   -> interrupts are disabled automatically
 *   -> guardian() runs
 *   -> Guard may run an epilogue
 *
 * Before running an epilogue, Task 3 wants interrupts to be enabled again.
 */
extern CPU cpu;

/*
 * leave()
 * -------
 * This method is called when normal kernel code leaves a protected
 * critical section.
 *
 * Example:
 *
 *   protected screen output starts
 *   -> Guard is marked occupied
 *   -> keyboard interrupt happens
 *   -> keyboard epilogue is queued
 *   -> protected screen output ends
 *   -> leave() is called
 *   -> queued keyboard epilogue can now run
 *
 * So leave() has two jobs:
 *
 *   1. Mark the critical section as free.
 *   2. Run all delayed epilogues that were waiting in the queue.
 */
void Guard::leave()
{
    /*
     * retne() is inherited from Locker.
     *
     * It marks the critical section as free:
     *
     *   free = true
     */
    retne();

    /*
     * Take the first waiting epilogue from the queue.
     *
     * queue.dequeue() returns a Chain*, because Queue stores Chain objects.
     * But we know that this queue contains Gate objects.
     *
     * Therefore we cast the returned Chain* back to Gate*.
     */
    Gate* item = (Gate*) queue.dequeue();

    /*
     * Process all queued epilogues.
     *
     * If the queue was empty, item is 0 and the loop is skipped.
     */
    while (item != 0) {
        /*
         * While an epilogue is running, we mark the Guard occupied again.
         *
         * This prevents another epilogue from being executed at the same
         * time through Guard::relay().
         */
        enter();

        /*
         * Interrupts were disabled before guardian() was called.
         *
         * Before executing the longer epilogue work, interrupts should be
         * enabled again. This allows new interrupt prologues to happen even
         * while an epilogue is running.
         */
        cpu.enable_int();

        /*
         * Run the delayed interrupt work.
         *
         * Example:
         * If item points to the keyboard object, this calls:
         *
         *   Keyboard::epilogue()
         */
        item->epilogue();

        /*
         * The epilogue is finished, so the Guard becomes free again.
         */
        retne();

        /*
         * Take the next waiting epilogue from the queue.
         *
         * If there are no more, dequeue() returns 0 and the loop ends.
         */
        item = (Gate*) queue.dequeue();
    }
}

/*
 * relay()
 * -------
 * This method is called when an interrupt prologue says:
 *
 *   "My epilogue should run."
 *
 * relay() receives the Gate object whose epilogue is needed.
 *
 * Example:
 *
 *   keyboard interrupt happens
 *   -> Keyboard::trigger() returns true
 *   -> guardian() calls guard.relay(&keyboard)
 *   -> relay() decides whether Keyboard::epilogue() runs now or later
 */
void Guard::relay(Gate* item)
{
    /*
     * avail() is inherited from Locker.
     *
     * It tells us whether a protected critical section is currently free.
     */
    if (avail()) {
        /*
         * If the critical section is free, the epilogue can run immediately.
         *
         * Before running it, enable interrupts again because guardian() was
         * entered with interrupts disabled.
         */
        cpu.enable_int();

        /*
         * Run the epilogue now.
         *
         * Because epilogue() is virtual, C++ calls the correct concrete
         * version, for example Keyboard::epilogue().
         */
        item->epilogue();
    } else {
        /*
         * If the critical section is occupied, the epilogue must not run now.
         *
         * Instead, store the Gate object in the queue. It will be processed
         * later when leave() is called.
         */
        queue.enqueue(item);
    }
}