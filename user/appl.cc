/*****************************************************************************/
/* Operating-System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         A P P L I C A T I O N                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Test application for demonstrating synchronization.                        */
/*****************************************************************************/

/* INCLUDES */
#include "user/appl.h"
#include "device/cgastr.h"
#include "guard/secure.h"

/*
 * Global output stream.
 *
 * kout is the shared screen output object.
 *
 * Both the normal application and keyboard epilogues can use kout.
 * That is why output operations must be protected in the fixed mode.
 */
extern CGA_Stream kout;

/*
 * action()
 * --------
 * Main activity of the test application.
 *
 * It repeatedly writes "a" and "A" at the same fixed screen position.
 *
 * The important detail:
 *
 *   Writing is not just one simple operation.
 *
 * The application:
 *
 *   1. saves the current cursor position
 *   2. moves the cursor to a fixed position
 *   3. writes a character
 *   4. flushes the output
 *   5. restores the old cursor position
 *
 * If a keyboard interrupt epilogue also uses the screen during these steps,
 * the cursor/output can become inconsistent.
 *
 * Parameter:
 *
 *   m == 1
 *      Mess mode.
 *      The output sequence is not protected.
 *
 *   m == 2
 *      Fix mode.
 *      The output sequence is protected with Secure/Guard.
 */
void Application::action(int m)
{
    /*
     * Fixed screen position where the test characters are written.
     */
    int x = 40;
    int y = 5;

    /*
     * Old cursor position.
     *
     * Before printing at (x, y), the application stores the current cursor
     * position here. After printing, it restores the cursor position.
     */
    int dx, dy;

    /*
     * First output sequence: print "a".
     *
     * In mode 2, Secure creates a protected critical section.
     *
     * Important:
     *
     *   Secure section;
     *
     * calls guard.enter() in its constructor.
     *
     * When the if-block ends, section is destroyed automatically, and its
     * destructor calls guard.leave().
     */
    if (m == 2) {
        Secure section;

        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "a";
        kout.flush();
        kout.setpos(dx, dy);
    } else {
        /*
         * Mode 1 intentionally does the same output without protection.
         * This keeps the old "Mess" test behavior.
         */
        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "a";
        kout.flush();
        kout.setpos(dx, dy);
    }

    /*
     * Delay loop.
     *
     * It makes the changing output visible and gives keyboard interrupts
     * time to happen between the two output sequences.
     */
    for (int i = 0; i < 2000000; i++) {
    }

    /*
     * Second output sequence: print "A".
     *
     * Same critical pattern as above.
     */
    if (m == 2) {
        Secure section;

        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "A";
        kout.flush();
        kout.setpos(dx, dy);
    } else {
        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "A";
        kout.flush();
        kout.setpos(dx, dy);
    }

    /*
     * Another delay before the next loop iteration.
     */
    for (int i = 0; i < 2000000; i++) {
    }
}