/*
 * Routines to support processes
 *
 * NOTE: at the moment, FoenixMCP is single tasking only, so there is only
 *       one "process" besides the kernel.
 *
 */

#include "errors.h"
#include "log.h"
#include "dev/fsys.h"

static const long k_default_stack = 0x00010000;     /* For now... we're just going to put the user stack under 0x00010000 */
static int g_proc_result;

/*
 * Assembly routine: reset the supervisor stack pointer and restart the CLI
 */
extern void restart_cli();

extern void call_user(long start, long stack, int argc, char * argv[]);

/*
 * Start a user mode process
 *
 * Inputs:
 * start = the address to start execution
 * stack = the location to start the user mode stack
 * argc = the number of parameters
 * argv = the array of parameters
 */
void proc_exec(long start, long stack, int argc, char * argv[]) {
    TRACE("proc_exec");

    log_num(LOG_INFO, "proc_exec start: ", start);
    log_num(LOG_INFO, "proc_exec stack: ", stack);

    g_proc_result = 0;
    call_user(start, stack, argc, argv);
}

/*
 * Quit the current user process
 *
 * NOTE: at the moment, this relaunches the CLI. In the future, this
 *       may cause execution to return to the program that started
 *       the user process.
 *
 * Inputs:
 */
void proc_exit(int result) {
    g_proc_result = result;
    restart_cli();
}

/*
 * Return the result code of the previously running user process
 */
int proc_get_result() {
    return g_proc_result;
}

/*
 * Find an executable binary matching the path, load it, and execute it
 *
 * Inputs:
 * path = the path to try to load
 * argc = the number of parameters
 * argv = the array of parameters
 *
 * Returns:
 * returns an error code on failure, will not return on success
 */
short proc_run(const char * path, int argc, char * argv[]) {

    TRACE("proc_run");

    /* TODO: allow for commands without extensions */
    /* TODO: allow for a search PATH */
    /* TODO: allocate stack more dynamically */

    long start = 0;
    short result = fsys_load(path, 0, &start);
    if (result == 0) {
        if (start != 0) {
            proc_exec(start, k_default_stack, argc, argv);
        } else {
            log_num(LOG_ERROR, "Couldn't execute file: ", result);
            return ERR_NOT_EXECUTABLE;
        }
    } else {
        log_num(LOG_ERROR, "Couldn't load file: ", result);
        return result;
    }
}
