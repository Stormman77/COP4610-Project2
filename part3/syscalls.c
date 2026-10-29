#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

/*
 * When elevator.ko is loaded, it puts its own functions in these kernel slots.
 * When it is unloaded.
 */
int (*STUB_start_elevator)(void) = NULL;
int (*STUB_issue_request)(int, int, int) = NULL;
int (*STUB_stop_elevator)(void) = NULL;

/* Let kernel modules see and fill in the slots above */
EXPORT_SYMBOL(STUB_start_elevator);
EXPORT_SYMBOL(STUB_issue_request);
EXPORT_SYMBOL(STUB_stop_elevator);

/*
 * System call 548
 */
SYSCALL_DEFINE0(start_elevator)
{
    if (STUB_start_elevator != NULL)
        return STUB_start_elevator();
    return -ENOSYS;   /* error: no module loaded */
}

/*
 * System call 549 (with 3 parameters).
 * Format is: type, name, type, name, type, name
 */
SYSCALL_DEFINE3(issue_request, int, start_floor, int, destination_floor, int, type)
{
    if (STUB_issue_request != NULL)
        return STUB_issue_request(start_floor, destination_floor, type);
    return -ENOSYS;
}

/* System call 550 (0 arguments)*/
SYSCALL_DEFINE0(stop_elevator)
{
    if (STUB_stop_elevator != NULL)
        return STUB_stop_elevator();
    return -ENOSYS;
}