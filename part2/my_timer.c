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

static struct proc_dir_entry* timer_entry; // ptr to proc entry
static struct timespec64 last_time; // Keep track of last valid time for elapsed time
static bool first_read = true; // Flag for output purposes
static char msg[256];
static size_t msg_len;

// Define proc read, called when proc/timer is read
static ssize_t timer_read(struct file* file, char * ubtf, size_t count, loff_t *ppos)
{
    int len = 0;

    if (*ppos > 0)
    { // No need to execute, since read iterates until return 0
        return 0;
    }

    // Get current time
    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);

    len += scnprintf(msg + len, 256 - len, "current time: %lld.%09ld\n", 
                (long long)current_time.tv_sec, current_time.tv_nsec);

    if (!first_read)
    {
            long long elapsed_sec = current_time.tv_sec - last_time.tv_sec;
            long elapsed_nsec = current_time.tv_nsec - last_time.tv_nsec;

            // Handle nsec underflow
            if (elapsed_nsec < 0)
            {
                elapsed_sec--;
                elapsed_nsec += 1000000000L;
            }

            len += scnprintf(msg + len, 256 - len, "elapsed time: %lld.%09ld seconds\n",
                            elapsed_sec, elapsed_nsec);
    }
    else first_read = false;
    msg_len = len;
    last_time = current_time;
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

// Set proc file read, writes aren't necessary
static const struct proc_ops timer_fops = {
    .proc_read = timer_read
};

// Initialize timer file with "ENTRY_NAME" timer
static int __init timer_init(void)
{
    timer_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &timer_fops);
    if (!timer_entry) {
        return -ENOMEM;
    }
    return 0;
}

// Called on exit, remove timer file
static void __exit timer_exit(void)
{
    proc_remove(timer_entry);
}

module_init(timer_init);
module_exit(timer_exit);