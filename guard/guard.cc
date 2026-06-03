#include "guard/guard.h"
#include "machine/cpu.h"

extern CPU cpu;

/* Leave the protected section and run queued epilogues. */
void Guard::leave()
{
    /* Queue operations must not be interrupted. */
    cpu.disable_int();
    retne();

    Gate* item = (Gate*) queue.dequeue();

    /* Run all epilogues that were delayed by a protected section. */
    while (item != 0) {
        item->queued(false);
        enter();

        /* Epilogues may take longer, so allow new interrupt prologues. */
        cpu.enable_int();
        item->epilogue();

        /* Protect the next queue access again. */
        cpu.disable_int();
        retne();
        item = (Gate*) queue.dequeue();
    }

    cpu.enable_int();
}

/* Run an epilogue now, or queue it if the Guard is occupied. */
void Guard::relay(Gate* item)
{
    if (avail()) {
        enter();
        cpu.enable_int();
        item->epilogue();
        leave();
    } else {
        /* Queue each gate at most once while the Guard is occupied. */
        cpu.disable_int();
        if (!item->queued()) {
            queue.enqueue(item);
            item->queued(true);
        }
    }
}
