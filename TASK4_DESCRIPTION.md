# Task 4: Context Switching for OOStuBS

Official assignment:
https://os.inf.tu-dresden.de/Studium/OSC/SS2026/tasks/task4/index.php

## Goal

Task 4 introduces simple cooperative process management to OOStuBS. Processes
act as coroutines and explicitly release the processor. The task builds the
complete path from saving a CPU context to selecting and running processes
through a scheduler.

The implementation must distinguish between:

- passive objects, which are called and return normally
- active objects, which own a stack and an independent flow of control

## Learning Objectives

- Refresh the x86-64 assembler needed for context switching.
- Understand how a process context is created, saved, restored, and switched.
- Understand the difference between active and passive objects.
- Build cooperative scheduling in layers: coroutine, dispatcher, and scheduler.

## Required Components

### `toc`

The thread-of-control structure stores the non-volatile CPU state of a
coroutine. Implement:

- `toc_settle()` to prepare a new stack and initial context
- `toc_go()` to start the first context
- `toc_switch()` to save the current context and restore another one

The definitions in `machine/toc.h` and the assembler offsets in
`machine/toc.inc` must remain consistent.

### `kickoff`

`kickoff()` is the first function executed by a newly started coroutine. It
calls the coroutine's virtual `action()` method.

It must never return because there is no normal caller or valid return address
above it. If `action()` finishes, `kickoff()` must handle that situation
without returning into an undefined address.

### `Coroutine`

`Coroutine` is the basic active-object abstraction. Each coroutine owns a
saved `toc` and receives its own stack.

Required operations:

- initialize a new context in the constructor
- start the first coroutine with `go()`
- switch from the current coroutine to another with `resume()`
- provide a virtual `action()` method for the coroutine's activity

### `Dispatcher`

`Dispatcher` tracks the coroutine that currently owns the processor.

Required operations:

- `go()` records and starts the first coroutine
- `dispatch()` switches from the active coroutine to another
- `active()` returns the currently active coroutine

Part b should use one global dispatcher so context switching is available
throughout OOStuBS.

### `Entrant`

`Entrant` represents a coroutine managed by the scheduler. It combines:

- `Coroutine`, for an independent stack and control flow
- `Chain`, so the process can be stored in a `Queue`

The application and all scheduled user processes must derive from `Entrant`.

### `Scheduler`

`Scheduler` extends `Dispatcher` with a ready queue of runnable `Entrant`
objects. Scheduling is cooperative and uses a simple first-come,
first-served/round-robin style.

Required operations:

- `ready(Entrant&)` adds a process to the ready queue
- `schedule()` starts scheduling with the first ready process
- `resume()` yields the processor and schedules another ready process
- `exit()` removes the active process from scheduling
- `kill(Entrant&)` removes a selected process

The final system must provide one global instance named `scheduler`.

### `Application`

`Application` is OOStuBS's first user process. It must derive from `Entrant`,
create additional user processes, register them with the global scheduler, and
demonstrate cooperative execution.

## Implementation Stages

### Part A: Coroutine Switching

1. Implement the `toc` access functions and initial stack setup.
2. Implement `kickoff()` and `Coroutine`.
3. Create several coroutine-based test processes.
4. Let each process execute briefly and explicitly pass control to the next.

At this stage, coroutines may know their successors directly.

### Part B: Dispatcher

1. Implement `Dispatcher`.
2. Create a global dispatcher.
3. Route all coroutine switches through `Dispatcher::dispatch()`.
4. Verify that `active()` always identifies the running coroutine.

### Part C: Scheduler

1. Implement `Entrant` and `Scheduler`.
2. Replace the global dispatcher with the global `scheduler`.
3. Make `Application` and all user processes derive from `Entrant`.
4. Register runnable processes using `ready()`.
5. Let processes yield using `Scheduler::resume()` without knowing which
   process runs next.

## Required Demonstration

The test program in `main.cc` must:

- create the global `scheduler`
- create the first global user process, `application`
- let the application create or register further user processes
- show repeated cooperative switches between independent process stacks
- demonstrate `Scheduler::exit()`
- demonstrate `Scheduler::kill(Entrant&)`
- test what happens when the active process kills itself

The output should make the execution order visible and confirm that local
variables and stacks survive context switches.

## Important Constraints

- Every active process needs its own stack with sufficient size and correct
  alignment.
- The System V AMD64 ABI's non-volatile registers must survive a switch.
- Stack preparation must match the calling convention expected by `kickoff()`.
- A process must not be inserted into the ready queue more than once.
- The scheduler must not dispatch a process that has exited or been killed.
- Queue removal and self-termination must not leave the active pointer or ready
  queue in an inconsistent state.
- `Chain` and `Queue` are supplied by the template and should be reused.

## Completion Checklist

- [ ] `toc_settle()`, `toc_go()`, and `toc_switch()` work correctly.
- [ ] `kickoff()` starts `action()` and never returns.
- [ ] Coroutines preserve their stacks and local state across switches.
- [ ] `Dispatcher` tracks the active coroutine correctly.
- [ ] `Entrant` can be stored in the scheduler's ready queue.
- [ ] `Scheduler` supports `ready`, `schedule`, `resume`, `exit`, and `kill`.
- [ ] A global `scheduler` replaces the temporary global dispatcher.
- [ ] `Application` and worker processes derive from `Entrant`.
- [ ] The test covers normal yielding, process exit, killing another process,
      and self-killing.
- [ ] The project builds and runs on the target environment.
