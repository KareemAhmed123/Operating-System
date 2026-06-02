/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                L O C K E R                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Simple lock state for protecting critical sections.                       */
/*****************************************************************************/

#ifndef __Locker_include__
#define __Locker_include__

/*
 * Requirements
 * ------------
 * Locker stores whether a critical section is currently available.
 *
 * It does not:
 * - wait for the lock
 * - manage epilogues
 * - enable or disable interrupts
 * - know anything about devices such as the keyboard
 *
 * Those responsibilities belong to higher-level classes such as Guard.
 *
 *
 * Architecture
 * ------------
 * Locker is the small base class for Guard.
 *
 * The intended layering is:
 *
 *   Locker
 *      |
 *      v
 *   Guard
 *
 * Locker only knows the current lock state:
 *
 *   free == true   -> critical section is available
 *   free == false  -> critical section is occupied
 *
 *
 * Design
 * ------
 * The methods are implemented inline because they are very small.
 *
 * enter():
 *   marks the critical section as occupied.
 *
 * retne():
 *   marks the critical section as free again.
 *
 * avail():
 *   returns whether the critical section is currently available.
 */
class Locker {
private:
    /*
     * true:
     *   no activity is currently inside the protected critical section
     *
     * false:
     *   a protected critical section is currently active
     */
    bool free;

public:
    /*
     * Create an initially available lock.
     */
    Locker()
    {
        free = true;
    }

    /*
     * Enter the critical section.
     *
     * After this call, the lock is no longer available.
     */
    void enter()
    {
        free = false;
    }

    /*
     * Leave the critical section.
     *
     * The method name retne is enter written backwards.
     * This naming is part of the OOStuBS task style.
     */
    void retne()
    {
        free = true;
    }

    /*
     * Return whether the critical section is available.
     */
    bool avail()
    {
        return free;
    }
};

#endif