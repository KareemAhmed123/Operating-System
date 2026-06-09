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
#include "thread/dispatch.h"


/*
 * Global output stream.
 *
 * kout is the shared screen output object.
 *
 * Both the normal application and keyboard epilogues can use kout.
 * That is why output operations must be protected in the fixed mode.
 */
extern CGA_Stream kout;
extern Guard guard;
// Defined in main.cc; all workers share this dispatcher so active() remains
// correct across every coroutine handoff.
extern Dispatcher dispatcher;

// New test Routin for toc and Coroutie
class Worker : public Coroutine {
private:
    char id;
    Coroutine* next;
    int limit;

public:
    Worker(void* tos, char id, int limit)
        : Coroutine(tos), id(id), next(0), limit(limit) {}

    void set_next(Coroutine* n) {
        next = n;
    }

    void action() override {
        int counter = 0;
        int local_test = id;

        while (counter < limit) {
            if (local_test != id) {
                kout << "STACK ERROR" << endl;
                while (1) {}
            }

            counter++;

            kout << id << counter << " ";
            kout.flush();

            // Yield to the configured successor. Using Dispatcher instead of
            // resume() directly also updates the globally active coroutine.
            dispatcher.dispatch(*next);
        }

        kout << id << " done ";
        kout.flush();

        while (1) {}
    }
};

static char stack_a[4096];
static char stack_b[4096];
static char stack_c[4096];


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
void Application::action()
{
	/*// Test for Task 2&3
    //Fixed screen position where the test characters are written.
    int x = 40;
    int y = 5;

    //Old cursor position.
    int dx, dy;

    //First output sequence: print "a".
	if (m == 2) {
        Secure section;

        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "a";
        kout.flush();
        kout.setpos(dx, dy);
    } else {
        //Mode 1 intentionally does the same output without protection.
        kout.getpos(dx, dy);
        kout.setpos(x, y);
        kout << "a";
        kout.flush();
        kout.setpos(dx, dy);
    }

    //Delay loop.
int i = 0; i < 2000000; i++) {
    }
    //Second output sequence: print "A".
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

    //Another delay 
    for (int i = 0; i < 2000000; i++) {
    }*/
    
    //New Test for Task4a
	Worker a(stack_a + sizeof(stack_a), 'A', 5);
    Worker b(stack_b + sizeof(stack_b), 'B', 5);
    Worker c(stack_c + sizeof(stack_c), 'C', 5);

    a.set_next(&b);
    b.set_next(&c);
    c.set_next(this);

    kout << "Coroutine test:" << endl;

    for (int i = 0; i < 5; i++) {
        kout << "[M" << i << "] ";
        kout.flush();

        // Transfer control from Application to worker A. The worker chain
        // eventually dispatches back to this Application coroutine.
        dispatcher.dispatch(a);
    }

    kout << endl << "Back in Application. Test finished." << endl;

    while (1) {}
}
