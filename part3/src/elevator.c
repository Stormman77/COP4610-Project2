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
#define BUFF_LEN 10000


#define MAX_WEIGHT 50
#define MAX_CAPACITY 5
#define NUM_FLOORS 5


// Types of pets, can use as if were index
#define CHIHUAHUA 0
#define PUG 1
#define PUGHUAHUA 2
#define DACHSCHUND 3


// Static constant variables
static const int pet_weights[] = {3, 14, 10, 16}; // Index corresponds to pet type


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
   bool should_stop;
} Elevator;


// Structure for a floor
typedef struct {
   int num_pets_waiting;
   struct list_head pets_waiting;
} Floor;


// Structure of a pet
typedef struct {
   int type;
   int start_floor;
   int destination_floor;
   struct list_head list; // Necessary for anything that could be a node in a list
} Pet;


// Global variables: should use mutex before accessing shared resources
static Elevator elevator;
static Floor floors[NUM_FLOORS];
static int total_pets_serviced = 0;
static int num_pets_waiting = 0;


// Procfile stuff
static struct proc_dir_entry *elevator_entry;
static char msg[256];
static size_t msg_len;




// Use this anywhere mutex is required
DEFINE_MUTEX(elevator_lock);


//
// End of declarations
//


// Helper function: get the string representation of the elevator state
static const char * get_state_string(States state)
{
   switch (state)
   {
       case OFFLINE: return "OFFLINE";
       case IDLE: return "IDLE";
       case LOADING: return "LOADING";
       case UP: return "UP";
       case DOWN: return "DOWN";
       default: return "UNKNOWN";
   }
}


// Helper function: get the char of a pet's type num
static char get_pet_char(int type)
{
   switch (type)
   {
       case CHIHUAHUA: return 'C';
       case PUG: return 'P';
       case PUGHUAHUA: return 'H';
       case DACHSCHUND: return 'D';
       default: return '?';
   }
}


// System call wrapper
int start_elevator(void)
{
   mutex_lock(&elevator_lock);
   // Set up elevator state
   elevator.current_floor = 1;
   elevator.state = IDLE;
   mutex_unlock(&elevator_lock);
   return 0;
}


// System call wrapper
int issue_request(int start_floor, int destination_floor, int type) {
   Pet *pet;
   if (start_floor < 1 || start_floor > NUM_FLOORS ||
       destination_floor < 1 || destination_floor > NUM_FLOORS ||
       type < 0 || type > 3 || start_floor == destination_floor)
           return 1; // Error case, shouldn't happen
   pet = kmalloc(sizeof(Pet), GFP_KERNEL);
   if (!pet) return -ENOMEM;
  
   pet->type = type;
   pet->start_floor = start_floor;
   pet->destination_floor = destination_floor;
   INIT_LIST_HEAD(&pet->list);


   // Add pet to end of start floor's waiting list
   mutex_lock(&elevator_lock);
   list_add_tail(&pet->list, &floors[start_floor - 1].pets_waiting);
   floors[start_floor - 1].num_pets_waiting++;
   num_pets_waiting++;
   mutex_unlock(&elevator_lock);


   return 0;
}


// System call wrapper
// Called when elevator should stop at next floor in scheduling algorithm
int stop_elevator(void) {
   mutex_lock(&elevator_lock);
   if (elevator.should_stop || elevator.state == OFFLINE)
   { // Elevator is already stopping OR elevator is offline, shouldn't set flag
       mutex_unlock(&elevator_lock);
       return 1;
   }
   elevator.should_stop = true;
   mutex_unlock(&elevator_lock);
   return 0;
}


// Procfile read
// Display current state of elevator and floors
static ssize_t elevator_read(struct file* file, char * ubtf, size_t count, loff_t *ppos)
{
   int len = 0;
   Pet *pet;


   if (*ppos > 0)
   { // No need to execute, since read iterates until return 0
       return 0;
   }


   mutex_lock(&elevator_lock);


   len += scnprintf(msg + len, BUFF_LEN - len, "Elevator state: %s\n", get_state_string(elevator.state));
   len += scnprintf(msg + len, BUFF_LEN - len, "Current floor: %d\nCurrent load: %d\n",
                                           elevator.current_floor, elevator.current_weight);


   // Todo: output list of pets on elevator before listing pets on each floor
   // Do once stuff can move in and out of elevator


   // This outputs the pets waiting on each floor
   for (int i = NUM_FLOORS - 1; i >= 0; i--)
   {
       len += scnprintf(msg + len, BUFF_LEN - len, "[%c] Floor %d: %d ",
                           elevator.current_floor == i+1?'*':' ', i+1, floors[i].num_pets_waiting);
       list_for_each_entry(pet, &floors[i].pets_waiting, list)
       {
           len += scnprintf(msg + len, BUFF_LEN - len, "%c%d ", get_pet_char(pet->type), pet->destination_floor);
       }
       len += scnprintf(msg + len, BUFF_LEN - len, "\n");
   }
   len += scnprintf(msg + len, BUFF_LEN - len, "Number of pets: %d\n", elevator.num_pets);
   len += scnprintf(msg + len, BUFF_LEN - len, "Number of pets waiting: %d\n", num_pets_waiting);
   len += scnprintf(msg + len, BUFF_LEN - len, "Number of pets serviced: %d\n", total_pets_serviced);
  
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
   elevator.current_floor = 0;
   elevator.num_pets = 0;
   elevator.current_weight = 0;
   elevator.should_stop = false;
   INIT_LIST_HEAD(&elevator.pets_on_elevator);


   // Initialize floors
   for (int i = 0; i < NUM_FLOORS; i++)
   {
       INIT_LIST_HEAD(&floors[i].pets_waiting);
       floors[i].num_pets_waiting = 0;
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
   // Clean up lists
   Pet *pet, *dummy;
   mutex_lock(&elevator_lock);
   list_for_each_entry_safe(pet, dummy, &elevator.pets_on_elevator, list)
   {
      list_del(&pet->list);
      kfree(pet);
   }
   for(int i = 0; i < NUM_FLOORS; i++)
   {
      list_for_each_entry_safe(pet, dummy, &floors[i].pets_waiting, list)
      {
         list_del(&pet->list);
         kfree(pet);
      }
   }
   mutex_unlock(&elevator_lock);

   // Remove procfile
   proc_remove(elevator_entry);

   // Set function pointers to null
   STUB_start_elevator = NULL;
   STUB_issue_request = NULL;
   STUB_stop_elevator = NULL;
}


module_init(init_elevator);
module_exit(exit_elevator);