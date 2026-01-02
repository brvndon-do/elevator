# Elevator Simulator (C++) – Actor-Style Concurrency Design

This document summarizes the design decisions, terminology, and incremental steps discussed so far. The goal is to build a **small, controllable elevator simulation** that exercises:
- concurrency fundamentals
- mutex / condition variable correctness
- message passing
- invariants and ownership discipline

The design intentionally starts minimal and grows in complexity.

---

## 1. High-Level Goal

Build an elevator simulator in C++ that:
- uses **multiple threads**
- avoids unsafe shared-state access
- is easy to reason about and test
- mirrors real-world elevator concepts where useful

The simulator is *not* trying to be physically realistic at first. Correctness and clarity take priority over realism.

---

## 2. Chosen Concurrency Model: Actor-Style

### Decision
Use an **actor-style concurrency model**.

### Rationale
- Each elevator runs in its **own thread**
- Each elevator **owns its state exclusively**
- Other threads communicate with elevators **only via messages**
- Shared mutable state is minimized

This reduces data races and makes invariants easier to enforce.

### Consequences
- No thread directly mutates another thread’s data
- All coordination happens through message queues (mailboxes)
- Mutexes and condition variables are localized to communication points

---

## 3. Core Terminology

### Hall Call
A request made **outside** the elevator.
- Example: `(floor = 5, direction = Up)`
- Direction is required because the destination is unknown

Handled by:
- A scheduler (later milestone)

### Car Call
A request made **inside** the elevator.
- Example: `(elevator = 1, floor = 10)`
- Destination floor is explicit; direction is implied

Handled by:
- The elevator itself

### Initial Scope Decision
For the first milestone:
- Only **car calls** are modeled
- `AddStop(floor)` is treated as a car call
- No scheduler yet

---

## 4. Time Model

### Tick-Based Simulation (Chosen)

Time advances in **explicit, discrete steps** controlled by the program.

- A "tick" represents one logical unit of time
- Elevator moves **at most one floor per tick**
- No dependency on wall-clock time or OS scheduling

Benefits:
- Deterministic behavior
- Easy to test and replay
- No reliance on `sleep_for`

Implementation approach:
- A controlling thread sends `Step` messages
- Elevators update state only when receiving `Step`

### Sleep-Based Simulation (Deferred)

Uses real time via OS sleeps.
- More realistic
- Harder to test
- Nondeterministic wakeup ordering

Can be added later without changing core logic.

---

## 5. Invariants

### Definition
An **invariant** is a condition that must always be true when the system is in a stable state.

Invariants are the primary tool for reasoning about correctness in concurrent systems.

### Key Invariants (So Far)

1. **Ownership invariant**
   - Elevator state is mutated **only by the elevator thread**

2. **Floor bounds invariant**
   - `0 <= currentFloor < numFloors`

3. **Stop ownership invariant**
   - The `stops` container is only modified via messages processed by the elevator thread

4. **Immediate satisfaction invariant**
   - If `AddStop(currentFloor)` is received, the request is treated as already satisfied

Invariants should be documented near the data they constrain and asserted where practical.

---

## 6. Mailbox (Message Queue)

### What a Mailbox Is

A **mailbox** is a thread-safe message queue used for inter-thread communication.

- Multiple producers (other threads)
- Single consumer (the elevator thread)
- Blocking receive

It is the fundamental building block of the actor model.

### Does the STL Provide This?

No.

The C++ standard library provides:
- Containers (`std::queue`, `std::deque`)
- Synchronization primitives (`std::mutex`, `std::condition_variable`)

The mailbox is a **custom data structure** built from these primitives.

---

## 7. Mailbox Data Structure

### Required Properties

- Thread-safe
- Blocks efficiently when empty
- No busy-waiting
- No missed wakeups

### Minimal Interface

- `push(message)`
- `pop_blocking() -> message`
- Optional: `try_pop()`

### Canonical Implementation Pattern

- Internal `std::queue<T>`
- Protected by `std::mutex`
- `std::condition_variable` for blocking waits

All waits must use a predicate to guard against spurious wakeups.

---

## 8. Elevator Actor (First Milestone)

### Elevator Thread Responsibilities

- Own all elevator state
- Process incoming messages sequentially
- Maintain invariants

### Elevator-Owned State

- `currentFloor`
- `direction` (Up / Down / Idle)
- `stops` (set of destination floors)

No other thread may directly access or mutate this state.

---

## 9. Messages

### Initial Message Set

For the first milestone:

- `AddStop(floor)`
- `Shutdown`

Later additions:
- `Step`
- Snapshot / query messages
- Scheduler-related messages

---

## 10. Control Flow (First Milestone)

1. Main thread creates an `Elevator` and starts its thread
2. Main thread sends `AddStop(floor)` messages
3. Elevator thread:
   - Blocks on mailbox
   - Processes messages sequentially
   - Updates internal state
4. Main thread eventually sends `Shutdown`
5. Elevator thread exits cleanly

At this stage:
- No scheduler
- No hall calls
- No real time

---

## 11. Next Incremental Steps (Planned)

1. Add `Step` message
2. Implement tick-based movement logic
3. Add door/open/idle semantics if desired
4. Introduce scheduler thread
5. Add hall calls and assignment policy
6. Add fairness and starvation prevention

Each step should introduce **one new concept** and preserve existing invariants.

---

## 12. Design Philosophy

- Constrain the world aggressively
- Introduce complexity only when necessary
- Prefer ownership and message passing over shared state
- Write invariants before code
- Make incorrect states unrepresentable where possible

This approach mirrors how real concurrent systems are designed when correctness matters.

