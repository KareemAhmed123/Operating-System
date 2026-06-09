# Task 3 Progress Notes

This document records what we have done so far for Task 3 and the reasoning
behind each step.

## Goal Of Task 3

Task 3 changes the synchronization in OOStuBS from hard interrupt locking to
the prologue/epilogue model.

In Task 2, critical sections were protected directly with:

```cpp
cpu.disable_int();
...
cpu.enable_int();
```

For Task 3, these calls should mostly disappear. Instead, interrupt handling is
split into:

- prologue: a short interrupt handler part
- epilogue: delayed work that runs later in a protected way

## Core Concepts

### The Original Problem

An interrupt can happen almost anywhere.

Example:

1. The main application saves the current cursor position.
2. It moves the cursor to another position.
3. It starts printing something.
4. A keyboard interrupt happens.
5. The keyboard handler also uses the screen and cursor.

Now two activities touch the same shared object, the screen output, at the same
time. This can break the cursor position or mix the output.

In Task 2, this was solved with hard synchronization:

```cpp
cpu.disable_int();
critical section
cpu.enable_int();
```

This works, but it is a very direct solution. While interrupts are disabled, the
processor cannot react normally to hardware interrupts.

### Prologue

The prologue is the first, immediate part of interrupt handling.

It runs directly when the interrupt arrives. Because it runs in interrupt
context, it should be short and simple.

Typical prologue work:

- notice that an interrupt happened
- acknowledge the interrupt if needed
- schedule later work
- return quickly

Typical work that should not be done in the prologue:

- long calculations
- complicated output
- touching shared state for a long time
- waiting
- loops

For this task, `prologue()` is the natural prologue method of a `Gate`.

Example idea:

```cpp
bool Keyboard::prologue()
{
    return true;
}
```

### Epilogue

The epilogue is the delayed part of interrupt handling.

It runs after the urgent interrupt entry work has finished, at a safer point.
The epilogue can do the larger work that should not happen directly inside the
interrupt prologue.

For the keyboard, epilogue work can include:

- reading the pressed key
- checking whether the key is valid
- printing a character
- moving the cursor
- handling special keys

In the final code, `Keyboard::prologue()` only returns whether epilogue work is
required. The keyboard handling itself is in `Keyboard::epilogue()`.

### Why Split Interrupt Handling?

The split keeps interrupt handling responsive.

Instead of this:

```text
interrupt happens
    -> do all keyboard work immediately
    -> return
```

Task 3 wants this:

```text
interrupt happens
    -> prologue: schedule keyboard work quickly
    -> return from urgent interrupt path
    -> epilogue: do keyboard work safely later
```

The important idea is:

```text
The prologue reacts quickly.
The epilogue does the real work safely.
```

### Gate

A `Gate` is an object that handles an interrupt.

Examples:

- `Keyboard`
- `Panic`

The `Plugbox` maps an interrupt slot to a `Gate`.

`guardian()` asks the `Plugbox` for the right `Gate`, runs its prologue, and
relays requested epilogue work:

```cpp
Gate& item = plugbox.report(slot);
if (item.prologue()) {
    guard.relay(&item);
}
```

A `Gate` must also be placeable in an epilogue queue. That is why `Gate`
inherits from `Chain`:

```cpp
class Gate : public Chain
```

Then objects like `Keyboard` can be stored in a `Queue`.

### Chain

`Chain` is the small base class that gives objects a `next` pointer:

```cpp
class Chain {
public:
    Chain* next;
};
```

This makes it possible to connect objects in a singly linked list.

Since `Queue` stores `Chain` objects, anything that should be put into the queue
must inherit from `Chain`.

### Queue

`Queue` stores `Chain` objects.

For Task 3, this queue will be used as the epilogue queue.

Conceptually:

```text
Guard
  epilogue queue:
    Keyboard -> another Gate -> another Gate -> 0
```

When an interrupt occurs, the corresponding `Gate` can be inserted into the
queue. Later, the `Guard` removes queued gates and runs their epilogues.

### Guard

`Guard` is the central synchronization object for Task 3.

It has two main jobs:

1. protect critical sections
2. manage the epilogue queue

Instead of manually disabling and enabling interrupts in application code, the
application should use the guard mechanism.

Rough mental model:

```text
Guard::enter()
    enter protected section

Guard::leave()
    leave protected section
    maybe run pending epilogues
```

The exact implementation still comes later.

### Secure

`Secure` is a helper object for critical sections.

The idea is to write:

```cpp
{
    Secure section;
    critical section
}
```

When the `Secure` object is created, it enters the guard.

When the block ends, the `Secure` object is destroyed automatically, and it
leaves the guard.

This C++ technique is called RAII:

```text
Resource Acquisition Is Initialization
```

For this task, the resource is the protected critical section.

The benefit is that leaving the critical section happens automatically, even if
the code returns early.

### Locker

`Locker` is the lower-level locking helper.

A useful mental model is:

```text
Locker: keeps track of whether the system is currently guarded
Guard: uses Locker and also handles epilogues
Secure: small automatic helper that enters/leaves Guard
```

The exact responsibilities depend on the class specification, but this is the
relationship between the concepts.

### guardian()

`guardian()` is the central interrupt entry function.

Before the Task 3 integration, `guardian()` directly called the handler:

```cpp
void guardian(unsigned int slot)
{
    plugbox.report(slot).trigger();
}
```

So it immediately calls the interrupt handler.

Task 3 adds an important detail:

```text
interrupts are disabled before guardian() is called
```

Therefore, interrupts must be enabled again manually at a suitable point.

This matters because if interrupts stay disabled forever, the keyboard and other
interrupt sources will stop working properly.

### Big Picture

Task 2 style:

```text
Application:
    disable interrupts
    use shared object
    enable interrupts

Keyboard interrupt:
    immediately does all keyboard work
```

Task 3 style:

```text
Application:
    enter guarded section
    use shared object
    leave guarded section

Keyboard interrupt:
    prologue schedules keyboard epilogue
    epilogue later does the keyboard work safely
```

The main lesson:

```text
Do the urgent interrupt reaction immediately.
Delay the larger interrupt work until it can run safely.
```

## Files We Inspected

We looked at the existing project structure and found these important files:

- `guard/gate.h`
- `guard/guardian.cc`
- `device/keyboard.h`
- `device/keyboard.cc`
- `user/appl.cc`
- `main.cc`

At the beginning, `Gate` was still very small:

```cpp
class Gate {
public:
    virtual void trigger() = 0;
};
```

At that point, `guardian()` also called the interrupt handler directly:

```cpp
plugbox.report(slot).trigger();
```

This became the integration point for the prologue/epilogue design.

## Hard Synchronization We Found

We searched for:

```bash
grep -R "disable_int\|enable_int" -n .
```

The important results were in:

- `user/appl.cc`
- `main.cc`

In `user/appl.cc`, the application still protects output using:

```cpp
cpu.disable_int();
...
cpu.enable_int();
```

This is Task 2 style. Later, this should be replaced by the Task 3 guard
mechanism.

## Why We Added Chain

The Task 3 class description says that `Gate` should derive from `Chain`.

`Chain` gives every derived object a `next` pointer, so objects can be stored in
a singly linked queue.

We created:

```text
object/chain.h
```

with:

```cpp
#ifndef __Chain_include__
#define __Chain_include__

class Chain {
public:
    Chain* next;
};

#endif
```

Meaning:

- every `Chain` object has a pointer to the next `Chain`
- if there is no next object, `next` should be `0`

## Why We Added Queue

Task 3 provides a queue concept for epilogues. The queue stores `Chain` objects.

We created:

```text
object/queue.h
object/queue.cc
```

The queue has:

```cpp
Chain* head;
Chain** tail;
```

The public methods are:

```cpp
Queue();
void enqueue(Chain* item);
Chain* dequeue();
void remove(Chain* item);
```

## Pointer Notes

`Chain*` means pointer to a `Chain`.

Example:

```cpp
Chain* head;
```

`head` stores the address of the first queue element.

`Chain**` means pointer to a pointer to a `Chain`.

Example:

```cpp
Chain** tail;
```

`tail` stores the address of a variable or field that stores a `Chain*`.

The two important pointer symbols are:

- `&` means "address of"
- `*` means "the value stored at this address"

Example:

```cpp
Chain** current = &head;
```

This means `current` points to the variable `head`.

Then:

```cpp
*current
```

means the value inside `head`.

## Queue Constructor

The constructor creates an empty queue:

```cpp
Queue::Queue()
{
    head = 0;
    tail = &head;
}
```

Reasoning:

- `head = 0` means the queue has no first element
- `tail = &head` means the next insertion should write into `head`

## Enqueue

`enqueue()` inserts an item at the end:

```cpp
void Queue::enqueue(Chain* item)
{
    item->next = 0;
    *tail = item;
    tail = &item->next;
}
```

Reasoning:

- the new item becomes the last element, so `item->next = 0`
- `*tail = item` inserts the item at the current insertion place
- `tail = &item->next` moves the insertion place to the new last element's
  `next` pointer

## Dequeue

`dequeue()` removes the first item:

```cpp
Chain* Queue::dequeue()
{
    Chain* item = head;

    if (item == 0) {
        return 0;
    }

    head = item->next;

    if (head == 0) {
        tail = &head;
    }

    item->next = 0;
    return item;
}
```

Reasoning:

- if `head` is `0`, the queue is empty
- otherwise, remember the first item
- move `head` to the next item
- if the queue became empty, reset `tail` to `&head`
- detach and return the removed item

## Remove

`remove()` removes a specific item:

```cpp
void Queue::remove(Chain* item)
{
    Chain** current = &head;

    while (*current != 0) {
        if (*current == item) {
            *current = item->next;

            if (*current == 0) {
                tail = current;
            }

            item->next = 0;
            return;
        }

        current = &((*current)->next);
    }
}
```

The key idea:

`current` is not the current element. It is the address of the pointer that
leads to the current element.

At the start:

```cpp
current = &head;
```

Later, it may become:

```cpp
current = &(some_element->next);
```

This lets the same code remove:

- the first item
- a middle item
- the last item

The important line is:

```cpp
*current = item->next;
```

It means:

- if `current == &head`, then `head = item->next`
- if `current == &(previous->next)`, then `previous->next = item->next`

So the item is bypassed.

## Current State

At this point:

- `object/chain.h` exists
- `object/queue.h` exists
- `object/queue.cc` exists
- `Queue::enqueue()` is implemented
- `Queue::dequeue()` is implemented
- `Queue::remove()` is implemented

## Connecting Gate To Chain

We then connected `Gate` to `Chain`.

Before:

```cpp
class Gate
```

After:

```cpp
#include "object/chain.h"

class Gate : public Chain
```

This means:

```text
Gate is a kind of Chain.
```

This is important because the epilogue queue stores `Chain` objects:

```cpp
void Queue::enqueue(Chain* item);
```

If `Gate` publicly inherits from `Chain`, then a `Gate*` can be used where a
`Chain*` is expected.

That gives us this relationship:

```text
Keyboard is a Gate
Gate is a Chain
therefore Keyboard is also usable as a Chain
therefore Keyboard can be inserted into the Queue
```

The word `public` matters:

```cpp
class Gate : public Chain
```

It means the outside world is allowed to treat a `Gate` as a `Chain`.

This is exactly what the `Guard` will need later when it puts interrupt handler
objects into the epilogue queue.

## Adding Prologue And Epilogue Methods To Gate

The Task 3 `Guard` description requires the prologue to indicate whether an
epilogue is needed:

```text
relay(Gate* item) is called if the previously executed prologue has indicated
by a return value of true that its epilogue should be executed.
```

Therefore, the prologue returns `bool`:

```cpp
virtual bool prologue() = 0;
```

The return value means:

```text
true
    this interrupt needs epilogue work

false
    no epilogue is needed
```

The meaning is:

- `prologue()` is the prologue method
- `epilogue()` is the delayed work method
- `queued()` tracks whether this Gate is already waiting in the queue

The final design is:

```cpp
class Gate : public Chain {
private:
    bool is_queued;

public:
    Gate()
    {
        next = 0;
        is_queued = false;
    }

    virtual bool prologue() = 0;
    virtual void epilogue() {}

    void queued(bool q)
    {
        is_queued = q;
    }

    bool queued()
    {
        return is_queued;
    }
};
```

The queued state is necessary because a `Gate` inherits only one `next` pointer
from `Chain`. Enqueueing the same Gate twice would reuse that pointer and
corrupt the queue.

## Splitting Keyboard Into Prologue And Epilogue

Before Task 3, `Keyboard::trigger()` did all keyboard work immediately:

- read the key
- check whether it is valid
- handle special keys
- move the cursor
- print characters
- possibly reboot

This is too much work for the prologue model.

We decided:

```text
The old big Keyboard::trigger() body should move to Keyboard::epilogue().
```

So the design becomes:

```text
Keyboard::prologue()
    short prologue
    later: schedule this keyboard object for epilogue work

Keyboard::epilogue()
    real keyboard work
```

The final split is:

```cpp
bool Keyboard::prologue()
{
    return true;
}

void Keyboard::epilogue()
{
    // keyboard handling logic
}
```

`prologue()` itself does not directly schedule the object. Instead, it returns
`true`. Then `guardian()` will call `guard.relay(...)`.

The important architectural step is done:

```text
The immediate interrupt method and the delayed work method are now separate.
```

## Design Questions We Answered

We checked the design before continuing.

Question:

```text
Should the big keyboard work stay in prologue(), or move to epilogue()?
```

Answer:

```text
Move to epilogue.
```

Question:

```text
What should prologue() do later?
```

Answer:

```text
Return true so guardian() knows the keyboard epilogue should be scheduled.
```

Question:

```text
Should we keep the old Task 2 menu with both modes?
```

Answer:

```text
Yes. Keep both modes.
```

So the application should still offer:

```text
1 Mess
2 Fix
```

This lets us compare the unprotected behavior with the protected Task 3
behavior.

## Locker Concept

Next, we looked at the `Locker` class description.

The important point:

```text
Locker only tracks whether the critical section is free.
```

It should not manage the epilogue queue.

It should not know about keyboard interrupts.

It should not decide when epilogues run.

Those bigger decisions belong to `Guard`.

The mental model is:

```text
Locker -> stores free/busy state
Gate   -> represents interrupt work
Guard  -> coordinates locking and epilogue execution
Secure -> automatic helper for entering/leaving Guard
```

The `Locker` class has one private boolean:

```cpp
bool free;
```

Meaning:

```text
free == true
    no one is inside the critical section

free == false
    someone is inside the critical section
```

The methods are short, so they belong directly inside `locker.h`:

```cpp
Locker()
{
    free = true;
}

void enter()
{
    free = false;
}

void retne()
{
    free = true;
}

bool avail()
{
    return free;
}
```

The method name `retne()` looks strange, but it is expected in OOStuBS. It is
`enter` written backwards and means leaving the critical section.

## Where We Are Now

Completed structure:

- `object/chain.h`
- `object/queue.h`
- `object/queue.cc`
- `guard/gate.h` provides prologue, epilogue, and queued-state handling
- `guard/locker.h` provides the critical-section state
- `guard/guard.h` and `guard/guard.cc` manage immediate and queued epilogues
- `guard/secure.h` provides scope-based critical sections
- `guard/guardian.cc` connects interrupt prologues to the Guard
- `device/keyboard.h` has both `prologue()` and `epilogue()`
- `device/keyboard.cc` has the old keyboard work moved into `epilogue()`
- `device/panic.h` and `device/panic.cc` use the same prologue/epilogue model

## Guard Concept

We then moved to the `Guard` class.

The Task 3 description says:

```text
Guard is used for synchronization between normal kernel activities and
interrupt handling routines.
```

The design is:

```text
Guard is a Locker plus an epilogue manager.
```

`Guard` inherits from `Locker`, so it can ask:

```cpp
avail()
```

Meaning:

```text
Is the critical section currently free?
```

`Guard` also owns a `Queue`:

```cpp
Queue queue;
```

This queue stores `Gate` objects whose epilogues cannot run immediately.

The class shape is:

```cpp
class Guard : public Locker {
private:
    Queue queue;

public:
    void leave();
    void relay(Gate* item);
};
```

## Guard::relay()

`relay()` is called when an interrupt prologue has said:

```text
Yes, my epilogue should run.
```

It receives the corresponding `Gate` object:

```cpp
void Guard::relay(Gate* item)
```

The decision rule is:

```text
if the critical section is free:
    run the epilogue immediately

if the critical section is occupied:
    put the Gate into the queue
```

Implementation:

```cpp
void Guard::relay(Gate* item)
{
    if (avail()) {
        enter();
        cpu.enable_int();
        item->epilogue();
        leave();
    } else {
        cpu.disable_int();
        if (!item->queued()) {
            queue.enqueue(item);
            item->queued(true);
        }
    }
}
```

The Guard is marked occupied before an immediate epilogue runs. This ensures
that another interrupt prologue queues its epilogue instead of starting a
second epilogue at the same time.

The `queued()` check prevents the same `Gate` object from being inserted twice.
This is required because each Gate has only one inherited `Chain::next` pointer.

Queue access happens while interrupts are disabled. Epilogues run with
interrupts enabled so that new interrupt prologues can still execute.

## Guard::leave()

`leave()` is called when normal kernel code leaves a protected critical section.

Its jobs are:

```text
1. mark the critical section as free
2. process all epilogues that were queued while the critical section was busy
```

Implementation:

```cpp
void Guard::leave()
{
    cpu.disable_int();
    retne();

    Gate* item = (Gate*) queue.dequeue();

    while (item != 0) {
        item->queued(false);
        enter();
        cpu.enable_int();
        item->epilogue();

        cpu.disable_int();
        retne();
        item = (Gate*) queue.dequeue();
    }

    cpu.enable_int();
}
```

Why process all queued epilogues?

If several interrupts happened while the critical section was occupied, several
epilogues may be waiting. When the critical section is released, this is the
safe time to process them.

Why call `enter()` while running an epilogue?

Because an epilogue itself may use shared kernel objects. While it runs, the
guard should be considered occupied, so another epilogue is not executed at the
same time through `relay()`.

Why clear `queued(false)` before running the epilogue?

The current queue entry has already been removed. If the same device interrupts
again while its epilogue runs, one new request may therefore be queued safely.

## Who Calls relay() And leave()?

`relay()` is called by `guardian()`.

Current `guardian()`:

```cpp
Gate& gate = plugbox.report(slot);

if (gate.prologue()) {
    guard.relay(&gate);
}
```

Flow:

```text
interrupt happens
    -> guardian(slot)
    -> find Gate through Plugbox
    -> call gate.prologue()
    -> if prologue() returns true, call guard.relay(&gate)
```

`leave()` is called by `Secure`.

The intended protected-section style is:

```cpp
{
    Secure section;
    critical section
}
```

When the `Secure` object is created, its constructor enters the guard.

When the block ends, its destructor calls:

```cpp
guard.leave();
```

So:

```text
guardian() calls relay()
Secure destructor calls leave()
```

## Guardian Integration

We updated `guard/guardian.cc` so it no longer ignores the return value of
`prologue()`.

Old idea:

```cpp
plugbox.report(slot).prologue();
```

New idea:

```cpp
Gate& item = plugbox.report(slot);

if (item.prologue()) {
    guard.relay(&item);
}
```

Meaning:

```text
1. Find the Gate for the interrupt slot.
2. Run its short prologue with prologue().
3. If the prologue returns true, give the Gate to Guard.
4. Guard decides whether the epilogue runs now or waits in the queue.
```

We also added:

```cpp
extern Guard guard;
```

This does not create the global Guard object. It only lets `guardian.cc` use
the global object that is defined elsewhere.

## Global Guard Object

We updated `main.cc` so the global Guard object really exists.

The includes now include:

```cpp
#include "guard/guard.h"
```

The global objects now include:

```cpp
CGA_Stream kout;
Plugbox plugbox;
PIC pic;
CPU cpu;
Guard guard;
```

This is necessary because several parts of the system must share the same
synchronization state:

```text
guardian.cc
    uses guard.relay() for interrupt epilogues

secure.h
    uses guard.enter() and guard.leave() for normal critical sections
```

If these used different Guard objects, the state would be inconsistent.

## Secure

We created `guard/secure.h`.

`Secure` is the helper used by normal code to mark critical sections.

It uses C++ object lifetime:

```cpp
{
    Secure section;
    critical section
}
```

Constructor:

```cpp
Secure()
{
    guard.enter();
}
```

Destructor:

```cpp
~Secure()
{
    guard.leave();
}
```

Meaning:

```text
when Secure object is created:
    critical section becomes busy

when Secure object leaves scope:
    critical section becomes free
    queued epilogues may run
```

This replaces the old Task 2 style:

```cpp
cpu.disable_int();
critical section
cpu.enable_int();
```

with the Task 3 style:

```cpp
{
    Secure section;
    critical section
}
```

## Application Update

We updated `user/appl.cc`.

The application still keeps both modes:

```text
1 Mess
2 Fix
```

Mode 1:

```text
same output code, no protection
```

Mode 2:

```text
same output code, protected with Secure/Guard
```

The critical output sequence is:

```text
save old cursor position
move to fixed position
print "a" or "A"
flush output
restore old cursor position
```

In mode 2, this sequence is wrapped in:

```cpp
Secure section;
```

When the block ends, the `Secure` destructor automatically calls:

```cpp
guard.leave();
```

So normal application output and interrupt epilogues now coordinate through the
same global Guard object.

## Panic Update

After changing `Gate::prologue()` from `void` to `bool`, `Panic` also had to be
updated.

Otherwise the compiler reported a return-type mismatch:

```text
Panic::prologue() returned void, but Gate::prologue() requires bool.
```

We changed `Panic` to follow the same prologue/epilogue structure:

```cpp
bool Panic::prologue()
{
    return true;
}

void Panic::epilogue()
{
    kout << "PANIC: Unhandled interrupt!" << endl;
    cpu.halt();
}
```

Meaning:

```text
Panic::prologue()
    short prologue, says epilogue should run

Panic::epilogue()
    real panic behavior: print message and halt
```

## Build Result

The final source state was checked with a forced rebuild:

```bash
make -B build/system
```

The kernel compiled and linked successfully:

```text
build/system
```

The linker still prints the existing warning that the system has an RWX load
segment. This warning is unrelated to the Task 3 Guard changes.

## Source Commit

The completed Guard/prologue changes were committed as:

```text
1bd1734 Implement Task 3 guard epilogue handling
```

This commit contains:

- the `trigger()` to `prologue()` interface rename
- queued-state tracking in `Gate`
- duplicate queue-entry prevention
- interrupt-protected queue operations
- immediate and delayed epilogue execution through `Guard`
- concise comments in `guard/guard.cc`
