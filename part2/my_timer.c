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

// Define proc read, called when proc/timer is read
static ssize_t timer_read(struct file* file, char * ubtf, size_t count, loff_t *ppos)
{
    char buf[256];
    int len = 0;

    // Get current time
    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);

    if (first_read)
    { // /proc/timer not been read before, no elapsed time to output
        len = snprintf(buf, sizeof(buf), "current time: %lld.%09ld\n", 
                    (long long)current_time.tv_sec, current_time.tv_nsec);
        first_read = false;
    }
    else
    { // /proc/timer has been read before and last_time has been set, safe to access
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

// Set proc file read, writes aren't necessary
static const struct proc_ops timer_fops = {
    .proc_read = timer_read,
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