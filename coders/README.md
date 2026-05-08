*This project has been created as part of the 42 curriculum by fabiochaput.*

# Codexion - Quantum Compiling Simulation

## Description
**Codexion** is a concurrency and synchronization challenge based on the classic "Dining Philosophers" problem, reimagined in a high-stakes co-working environment. The project simulates a group of coders sitting around a circular table, competing for limited resources (Quantum USB Dongles) to compile their code.

The goal is to orchestrate multiple threads (coders) and shared resources (dongles) while avoiding deadlocks, preventing coder burnout, and adhering to strict arbitration policies (FIFO or EDF).

## Instructions

### Compilation
The project includes a `Makefile` with the following rules:
- `make`: Compiles the `codexion` executable.
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Performs a full re-compilation.

### Execution
Run the program with the following 8 mandatory arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Arguments:
- `number_of_coders`: Number of threads and dongles.
- `time_to_burnout` (ms): Max time between the start of two compilations.
- `time_to_compile` (ms): Duration of the compilation phase.
- `time_to_debug` (ms): Duration of the debugging phase.
- `time_to_refactor` (ms): Duration of the refactoring phase.
- `number_of_compiles_required`: Simulation stops when all coders reach this count.
- `dongle_cooldown` (ms): Delay before a released dongle becomes available again.
- `scheduler`: Arbitration policy, either `fifo` (First In, First Out) or `edf` (Earliest Deadline First).

## Blocking cases handled

- **Deadlock Prevention**: To avoid circular wait, coders always attempt to acquire the dongle with the lower address first. This hierarchy ensures that at least one coder can always proceed, preventing the system from freezing.
- **Starvation Prevention**: The `edf` (Earliest Deadline First) scheduler prioritizes coders closest to burning out, ensuring fair access to resources even under high contention.
- **Dongle Cooldown**: A specialized state within the dongle structure prevents it from being re-acquired until the specified cooldown period has elapsed, managed via timestamps.
- **Precise Burnout Detection**: A dedicated monitor thread continuously checks the state of all coders and ensures burnout logs are printed within a 10ms window of the actual event.
- **Log Serialization**: All output messages are protected by a global `print_mutex` to prevent line interleaving and ensure coherent logs.

## Thread synchronization mechanisms

- **pthread_mutex_t**: Used to protect the state of each dongle, coder-specific data (like `last_compile_start`), and shared simulation flags (the `stop` condition).
- **pthread_cond_t**: Condition variables are used to put threads to sleep while waiting for a dongle to become available or for its cooldown to expire, reducing CPU consumption compared to busy-waiting.
- **Custom Priority Queue (Heap)**: A binary heap implementation manages the waiting queue for each dongle, allowing the scheduler to efficiently select the next coder based on `fifo` or `edf` policies.
- **Thread-safe Communication**: The monitor thread communicates with coder threads through a synchronized `stop` flag. When a burnout is detected, the monitor sets the flag and broadcasts to all condition variables to wake up and terminate all threads gracefully.

## Resources
- [POSIX Threads Documentation](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

### AI Usage
AI was used in this project for:
- Structuring the initial data models and simulation logic.
- Implementing the binary heap functions for the priority queue.
- Generating the project documentation and performing an audit of the concurrency logic to identify potential race conditions.
