#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 22");
MODULE_DESCRIPTION("A pet elevator module that moves pets up and down.");

#define ENTRY_NAME "elevator"
#define PERMS 0666
#define PARENT NULL

#define MAX_WEIGHT 50
#define MAX_CAPACITY 5
#define NUM_FLOORS 5

// Types of pets, can use as if were index
#define CHIHUAHUA 0
#define PUG 1
#define PUGHUAHUA 2
#define DACHSCHUND 3

// Static constant variables
static const int pet_weights[] = {3, 14, 10, 16};
static const char *pet_names[] = {"Chihuahua", "Pug", "Pughuahua", "Dachshund"};

// Syscall function pointers, defined in kernel
extern int (*STUB_start_elevator)(void);
extern int (*STUB_issue_request)(int,int,int);
extern int (*STUB_stop_elevator)(void);

// Function prototypes
int start_elevator(void);
int issue_request(int start_floor, int destination_floor, int type);
int stop_elevator(void);   

// Possible elevator states
typedef enum {
    OFFLINE, 
    IDLE, 
    LOADING, 
    UP, 
    DOWN
} States;

// Structure for elevator
typedef struct {
    int current_floor;
    int num_pets;
    int current_weight;
    struct list_head pets_on_elevator;
    States state;
} Elevator;

// Structure for a floor
typedef struct {
    struct list_head pets_waiting;
} Floor;

// Global variables: should use mutex before accessing shared resources
static Elevator elevator;
static Floor floors[NUM_FLOORS];
static int total_pets_serviced = 0;
static struct proc_dir_entry *elevator_entry;
static char msg[256];
static size_t msg_len;


// Use this anywhere mutex is required
DEFINE_MUTEX(elevator_lock);

//
// End of declarations
//

int start_elevator(void) 
{
    mutex_lock(&elevator_lock);
    // Set up elevator state
    elevator.current_floor = 1;
    elevator.num_pets = 0;
    elevator.current_weight = 0;
    elevator.state = IDLE;
    mutex_unlock(&elevator_lock);
    return 0;
}

int issue_request(int start_floor, int destination_floor, int type) {
    return 0;
}

int stop_elevator(void) {
    return 0;
}

// Called when proc file is read
// Display current state of elevator
static ssize_t elevator_read(struct file* file, char * ubtf, size_t count, loff_t *ppos)
{
    int len = 0;

    if (*ppos > 0)
    { // No need to execute, since read iterates until return 0
        return 0;
    }

    mutex_lock(&elevator_lock);

    len += scnprintf(msg + len, 256 - len, "Elevator is on floor %d.\n", elevator.current_floor);
    if (elevator.state == IDLE)
    {
        len += scnprintf(msg + len, 256 - len, "Elevator is idle.\n");
    }
    else if (elevator.state == OFFLINE)
    {
        len += scnprintf(msg + len, 256 - len, "Elevator is offline.\n");
    }
    msg_len = len;
    *ppos = msg_len;

    mutex_unlock(&elevator_lock);

    if (count < msg_len)
    {
        return 0;
    }
    if (copy_to_user(ubtf, msg, msg_len))
    {
        return -EFAULT;
    }
    *ppos = msg_len;
    return msg_len;
}

// Elevator fileops
static const struct proc_ops elevator_fops = {
    .proc_read = elevator_read
};

// Open elevator
static int __init init_elevator(void)
{
    // Initialize elevator
    elevator.state = OFFLINE;
    elevator.current_floor = 1;
    elevator.num_pets = 0;
    elevator.current_weight = 0;
    INIT_LIST_HEAD(&elevator.pets_on_elevator);

    // Initialize floors
    for (int i = 0; i < NUM_FLOORS; i++)
    {
        INIT_LIST_HEAD(&floors[i].pets_waiting);
    }

    // Create procfile
    elevator_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &elevator_fops);
    if (!elevator_entry) {
        return -ENOMEM;
    }

    // Set syscall function pointers
    STUB_start_elevator = start_elevator;
	STUB_issue_request = issue_request;
	STUB_stop_elevator = stop_elevator;

    return 0;
}

// Close elevator
static void __exit exit_elevator(void)
{
    proc_remove(elevator_entry);

    STUB_start_elevator = NULL;
	STUB_issue_request = NULL;
	STUB_stop_elevator = NULL;
}

module_init(init_elevator);
module_exit(exit_elevator);