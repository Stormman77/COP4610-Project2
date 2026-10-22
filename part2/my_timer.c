#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devan Erle");
MODULE_DESCRIPTION("Keeps track of the elapsed time since the last call.");

#define ENTRY_NAME "timer"
#define PERMS 0666
#define PARENT NULL

#define BUF_LEN 256 // Max length of read/write message
static struct proc_dir_entry* timer_entry; // ptr to proc entry
//static char msg[BUF_LEN]; // Buffer to store read/write messages
//static int procfs_buf_len; // Var to hold length of message
static struct timespec64 last_time; // Keep track of last valid time
static bool first_read = true;

// Define proc read, called when proc/timer is read
static ssize_t timer_read(struct file* file, char * ubtf, size_t count, loff_t *ppos)
{

    char buf[256];
    int len = 0;

    // Get current time
    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);
    
    if (first_read)
    { // /proc/timer not been read before, set to false
        len = snprintf(buf, sizeof(buf), "current time: %lld.%09ld\n", 
                    (long long)current_time.tv_sec, current_time.tv_nsec);
        first_read = false;
    }
    else
    { // Prior /proc/timer entries exist, can access last time
        long long elapsed_sec = current_time.tv_sec - last_time.tv_sec;
        long elapsed_nsec = current_time.tv_nsec - last_time.tv_nsec;

        // Handle nsec underflow
        if (elapsed_nsec < 0)
        {
            elapsed_sec--;
            elapsed_nsec += 1000000000L;
        }

        len = snprintf(buf, sizeof(buf), "current time: %lld.%09ld\nelapsed time: %lld.%09ld seconds\n",
                        (long long)current_time.tv_sec, current_time.tv_nsec, 
                        elapsed_sec, elapsed_nsec);
    }
    last_time = current_time;
    return simple_read_from_buffer(ubtf, count, ppos, buf, len);
}

// Set proc read
static const struct proc_ops timer_fops = {
    .proc_read = timer_read,
};

static int __init timer_init(void)
{
    timer_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &timer_fops);
    if (!timer_entry) {
        return -ENOMEM;
    }
    return 0;
}

static void __exit timer_exit(void)
{
    proc_remove(timer_entry);
}

module_init(timer_init);
module_exit(timer_exit);