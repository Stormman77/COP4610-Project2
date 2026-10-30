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
- **Responsibilities**: 
- **Assigned to**: 

### Part 3B: Kernel Compilation
- **Responsibilities**: 
- **Assigned to**: 

### Part 3C: Threads
- **Responsibilities**: 
- **Assigned to**: 

### Part 3D: Linked List
- **Responsibilities**: Use implementation of linux/list.h to keep track of pets on each floor and in elevator.
- **Assigned to**: Devan Erle

### Part 3E: Mutexes
- **Responsibilities**: Use implementation of linux/mutex.h to protect shared resources between threads
- **Assigned to**: Devan Erle

### Part 3F: Scheduling Algorithm
- **Responsibilities**:
- **Assigned to**: 

## File Listing
```
COP4610-Project2/
│
├── part1/
│ ├── empty.c
│ ├── empty.trace
│ ├── part1.c
│ └── part1.trace
│
├── part2/
│ ├── Makefile
│ └── my_timer.c
│
└── README.md
```
<!-- ## How to Compile & Execute

### Requirements
- **Compiler**: gcc
- **Dependencies**: N/A

### Compilation
```bash
make
```
This will build the executable in /bin
### Execution
```bash
make run
```
OR
```bash
./bin/shell
```
This will run the program. -->

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

### [Travern Martini]

| Date       | Work Completed / Notes |
|------------|------------------------|
| XXXX-XX-XX | [item]  |


### [Alexey Cherepanov]

| Date       | Work Completed / Notes |
|------------|------------------------|
| XXXX-XX-XX | [item]  |


## Meetings
Document in-person meetings, their purpose, and what was discussed.

| Date       | Attendees            | Topics Discussed | Outcomes / Decisions |
|------------|----------------------|------------------|-----------------------|
| 2026-10-12 | [Alexey Cherepanov, Travern Martini, Devan Erle]              | Project requirements   | Timeline/Division of Labor  |



## Bugs
- **Bug 1**:
- **Bug 2**: 
- **Bug 3**: 
