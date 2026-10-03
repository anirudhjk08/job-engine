# Job Engine

A multithreaded job execution engine written in modern C++20.

The project implements a thread-safe job queue and a worker pool that
processes submitted jobs concurrently.

## Features

- Thread-safe job queue
- Producer-consumer architecture
- Configurable worker pool
- Graceful shutdown
- Job rejection after shutdown
- Exception isolation for individual jobs
- Thread-safe logging
- CMake-based build system
- Basic automated tests

## Architecture

```text
                    JobEngine
                       |
              +--------+--------+
              |                 |
          JobQueue           Logger
              |
        +-----+-----+
        |     |     |
     Worker Worker Worker
        |     |     |
        +-----+-----+
              |
           Execute
             Jobs
Components
Job
Represents a unit of executable work using std::function<void()>.
JobQueue
Thread-safe queue responsible for:
- Adding jobs
- Providing jobs to workers
- Synchronizing access using std::mutex
- Blocking workers using std::condition_variable
- Managing shutdown state
Worker
A worker owns a thread that continuously:
1. Waits for a job
2. Retrieves the job from the queue
3. Executes it
4. Handles job exceptions
5. Continues processing until shutdown
JobEngine
Provides the high-level interface for the system.
It manages:
- The job queue
- Worker threads
- Logger
- Engine lifecycle
- Job submission
- Shutdown
Logger
Provides thread-safe console logging using a mutex so multiple workers
can safely write log messages.
Concurrency Model
The engine follows a producer-consumer model.
Producer
   |
   v
JobQueue
   |
   +----> Worker 1
   |
   +----> Worker 2
   |
   +----> Worker 3

Multiple workers consume jobs from the same shared queue.
std::condition_variable prevents workers from continuously polling
an empty queue.
Shutdown
When shutdown is requested:
1. New job submissions are rejected.
2. The queue is marked as shutting down.
3. Waiting workers are notified.
4. Existing queued jobs are processed.
5. Workers exit.
6. The engine joins all worker threads.
Error Handling
Exceptions thrown by individual jobs are caught inside the worker.
A failed job therefore does not terminate the worker thread or the
entire worker pool.
Build
Configure the project:
cmake -S . -B build

Build:
cmake --build build

Run:
./build/job_engine.exe

Tests
Build the project and run:
./build/job_engine_test.exe

The tests currently verify job submission and rejection after shutdown.
Project Structure
job-engine/
├── include/
│   ├── Job.h
│   ├── JobQueue.h
│   ├── Worker.h
│   ├── JobEngine.h
│   └── Logger.h
├── src/
│   └── main.cpp
├── tests/
│   └── JobEngineTest.cpp
├── examples/
├── CMakeLists.txt
├── README.md
└── .gitignore

Technologies
- C++20
- CMake
- STL
- std::thread
- std::mutex
- std::condition_variable
- std::unique_ptr
- std::function
- std::optional
Future Work
- TCP client-server interface
- Remote job submission
- Concurrent client handling
- Job protocol and serialization
- Performance benchmarking