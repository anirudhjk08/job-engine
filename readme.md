# Job Engine

A multithreaded job execution engine built with modern C++20, featuring a thread-safe job queue, worker pool, exception isolation, and TCP-based remote job submission.

## Features

- Thread-safe job queue using `std::mutex` and `std::condition_variable`
- Producer-consumer architecture
- Configurable worker pool
- Concurrent job execution
- Graceful JobEngine shutdown
- Job rejection after shutdown
- Exception isolation for individual jobs
- Thread-safe logging
- TCP client-server communication
- Remote job submission through TCP
- Concurrent client handling
- CMake-based build system
- Automated tests

## Architecture

```text
                  TCP Client
                      |
                      | RUN_JOB
                      v
                +-----------+
                | TCP Server|
                +-----------+
                      |
                      v
                 JobEngine
                      |
              +-------+-------+
              |               |
          JobQueue          Logger
              |
        +-----+-----+-----+
        |           |     |
     Worker 1    Worker 2 Worker 3
        |           |     |
        +-----------+-----+
                    |
               Execute Job
```

## Components

### Job

Represents a unit of executable work using:

```cpp
std::function<void()>
```

This allows the engine to execute different callable tasks through a common interface.

### JobQueue

A thread-safe queue responsible for:

- Adding jobs
- Providing jobs to workers
- Synchronizing access with `std::mutex`
- Blocking workers with `std::condition_variable`
- Managing shutdown state

### Worker

Each worker owns a dedicated thread that continuously:

1. Waits for a job
2. Retrieves a job from the queue
3. Executes the job
4. Handles job exceptions
5. Continues processing until shutdown

### JobEngine

Provides the high-level interface for the system and manages:

- JobQueue
- Worker pool
- Logger
- Engine lifecycle
- Job submission
- Shutdown

### Logger

Provides thread-safe console logging using a mutex so multiple workers can safely write log messages concurrently.

### TcpServer

Provides TCP networking using Windows Winsock.

The server:

1. Listens for incoming TCP connections
2. Accepts clients
3. Receives job requests
4. Submits valid requests to `JobEngine`
5. Sends a response to the client
6. Handles multiple clients concurrently

## Concurrency Model

The engine follows a producer-consumer model.

```text
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
```

Multiple workers consume jobs from the same shared queue.

`std::condition_variable` allows workers to sleep while the queue is empty instead of continuously polling.

The queue is protected by a mutex to prevent concurrent access from causing data races.

## TCP Job Submission

A client can submit a job using the TCP protocol:

```text
Client
   |
   | "RUN_JOB"
   v
Server
   |
   v
JobEngine.submit()
   |
   v
Worker executes job
   |
   v
"JOB_ACCEPTED"
   |
   v
Client
```

Multiple clients can connect concurrently, with each client connection handled by a separate thread.

## Shutdown Behavior

When `JobEngine::shutdown()` is called:

1. New job submissions are rejected.
2. The queue enters shutdown state.
3. Waiting workers are notified.
4. Already queued jobs are processed.
5. Workers exit after the queue is drained.
6. The engine joins all worker threads.

## Error Handling

Exceptions thrown by individual jobs are caught inside the worker.

A failed job therefore does not terminate the worker thread or the entire worker pool.

Network errors such as failed socket creation, binding, listening, or connection acceptance are also checked and reported.

## Build

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

Run the local JobEngine example:

```bash
./build/job_engine.exe
```

## TCP Server and Client

Start the server:

```bash
./build/job_server.exe
```

Then run the client:

```bash
./build/job_client.exe
```

The client sends:

```text
RUN_JOB
```

and receives:

```text
JOB_ACCEPTED
```

The server submits the request to the JobEngine, where a worker executes the job.

## Tests

Build the project and run:

```bash
./build/job_engine_test.exe
```

The tests verify:

- Job submission
- Job execution
- Job rejection after shutdown

## Project Structure

```text
job-engine/
├── include/
│   ├── Job.h
│   ├── JobQueue.h
│   ├── Worker.h
│   ├── JobEngine.h
│   ├── Logger.h
│   └── TcpServer.h
├── src/
│   └── main.cpp
├── tests/
│   └── JobEngineTest.cpp
├── examples/
│   ├── server.cpp
│   └── client.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Technologies

- C++20
- CMake
- STL
- `std::thread`
- `std::mutex`
- `std::lock_guard`
- `std::unique_lock`
- `std::condition_variable`
- `std::unique_ptr`
- `std::function`
- `std::optional`
- Windows Winsock / TCP

## Future Improvements

- Structured job protocol and serialization
- Job IDs and job status tracking
- Persistent job results
- Performance benchmarking
- Configurable network worker limits
- More comprehensive automated and stress testing