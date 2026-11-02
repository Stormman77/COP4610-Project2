# Pet Elevator

## Group Members
- **Devan Erle**: de24a@fsu.edu
- **Traven Martini**: tvm23a@fsu.edu
- **Alexey Cherepanov**: ab19@fsu.edu
## Division of Labor

### Part 1: System-Call Tracing
- **Responsibilities**: Create empty.c, an empty program, and part1.c with 5 system calls. Run strace on both in a unix environment.
- **Assigned to**: Devan Erle

### Part 2: Kernel-Timer Module
- **Responsibilities**: Implement a custom kernel module that creates the file /proc/timer. When this file is read from, get the time with ktime_get_real_ts64(), and print the elapsed time since the last read call. Delete the file /proc/timer when unloaded.
- **Assigned to**: Devan Erle

### Part 3A: Adding System Calls
- **Responsibilities**: Create a .C file that defines three new system calls `start_elevator()`, `issue_request()`, and `stop_elevator()`
- **Assigned to**: Travern Martini

### Part 3B: Kernel Compilation
- **Responsibilities**: Configure the kernel for Linux-6.16.12 with the new system calls. Compile the kernel, and document the steps taken.
- **Assigned to**: Travern Martini

### Part 3C: Threads
- **Responsibilities**: 
- **Assigned to**: Alexey Cherepanov

### Part 3D: Linked List
- **Responsibilities**: Use implementation of linux/list.h to keep track of pets on each floor and in elevator.
- **Assigned to**: Devan Erle

### Part 3E: Mutexes
- **Responsibilities**: Use implementation of linux/mutex.h to protect shared resources between threads
- **Assigned to**: Devan Erle

### Part 3F: Scheduling Algorithm
- **Responsibilities**:
- **Assigned to**: Alexey Cherepanov

## File Listing
```
COP4610-Project2/
│
├── part1/
│ ├── Makefile
│ ├── empty.c
│ ├── empty.trace
│ ├── part1.c
│ └── part1.trace
│
├── part2/
│ ├── Makefile
│ └── my_timer.c
│
├── part3/
│ ├── src
│ │ ├── Makefile
│ │ └── elevator.c
│ ├── tests
│ │ ├── elevator_test
│ │ │ ├── Makefile
│ │ │ ├── README.md
│ │ │ ├── consumer.c
│ │ │ ├── producer.c
│ │ │ └── wrappers.h
│ │ └── system-calls-test
│ │ │ ├── Makefile
│ │ │ ├── README.md
│ │ │ ├── syscheck.c
│ │ │ ├── test-syscalls.c
│ │ └─└── test-syscalls.h
│ ├── Makefile
│ └── syscalls.c
│
└── README.md
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc
- **Kernel**: Linux-6.16.12, modified to support system calls defined in `/part3/syscalls.c`
- **Dependencies**: installation/configuration of the commands `make` and `strace`, as well as root access for parts 2 and 3

## Part 1

### Compilation
```bash
make
```
This will build the executables `empty` and `part1`
### Execution
```bash
make trace
```
These will create the files `empty.trace` and `part1.trace`, which list the system calls made at the bottom of the files.

## Part 2

### Compilation
```bash
make
```
This will build the kernel module `my_timer.ko`
### Execution
```bash
sudo insmod my_timer.ko
```
This will load the kernel module and create the proc file `timer`.
```bash
cat /proc/timer
```
This calls the proc file's read function, displaying the current time and the elapsed time since the last read, if applicable.
```bash
sudo rmmod my_timer.ko
```
This will unload the kernel module and delete the proc file `timer`.

## Part 3

### Compilation
```bash
make
```
This will build the kernel module with the directory `/src/elevator.ko` and the executables `consumer` and `producer` in the current directory, for easy execution.

### Execution
This process already assumes the proper installation and compilation of the Linux-6.16.12 kernel, modified to support the system calls seen in syscalls.c. Ensure the machine has booted the proper kernel before proceeding.

```bash
sudo insmod src/elevator.ko
```
This will load the kernel module and create the proc file `elevator`.

In a separate terminal, enter the command
```bash
watch -n 1 cat /proc/elevator
```
This will begin "watching" the proc file by reading from `/proc/elevator`every second. The elevator can be observed here.

In the original terminal, enter
```bash
./producer num
```
Where `num` is an arbitrary number of pets.

```bash
./consumer --start
```
This will start the elevator. It can now be observed moving in the terminal executing `watch`.

```bash
./consumer --stop
```
This will stop the elevator, and it will go offline.

```bash
sudo rmmod src/elevator.ko
```
This will unload the elevator module and delete the proc file `elevator`.

## Development Log

### [Devan Erle]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-20 | [Finished part 1 (strace), formatted README]  |
| 2026-10-22 | [Implemented kernel timer module in part 2, still need to revise]  |
| 2026-10-27 | [Cleaned up code, issue with part2 now exists where first_read flag is ignored]  |
| 2026-10-28 | [Debugged part 2, found out it was an issue with the return val: proc read executes until return val is 0]  |
| 2026-10-29 | [Began work on elevator.c]  |
| 2026-10-30 | [Implemented syscalls from syscalls.c in the elevator.c program and their consumer/producer operations, as well as procfile reads, linked list, mutex, elevator module structure and exit function with cleanup. Should be able to pick up proj and continue in any direction.]  |
| 2026-11-2 | [Created Makefiles for each part]  |

### [Travern Martini]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-28 | [Wrote file for system calls]  |
| 2026-10-28 | [Added proper files for kernel configuration, kernel successfully compiled with added calls]  |


### [Alexey Cherepanov]

| Date       | Work Completed / Notes |
|------------|------------------------|
| 2026-10-27 | worked on kthread  |
| 2026-10-29 | worked on scheduling algorithm, added small features like makefile and .gitignore  |
| 2026-10-30 | continued working on scheduling  |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2026-10-12 | [Alexey Cherepanov, Travern Martini, Devan Erle]              | Project requirements   | Timeline/Division of Labor  |



## Bugs
No known bugs
