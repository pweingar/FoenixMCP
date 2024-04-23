/*
 * Routines to support processes
 *
 * NOTE: at the moment, FoenixMCP is single tasking only, so there is only
 *       one "process" besides the kernel.
 *
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_ERROR
#endif

#include "errors.h"
#include "log.h"
#include "dev/fsys.h"
#include <stdlib.h>
#include <string.h>

static const long k_default_stack = 0x00010000;     /* For now... we're just going to put the user stack under 0x00010000 */
static int g_proc_result;

static int g_proc_buffer_size;                      /* How many bytes are in the parameter buffer */
static char * g_proc_buffer;                        /* Pointer to a buffer for all the parameter data */

/*
 * Assembly routine: reset the supervisor stack pointer and restart the CLI
 */
extern void restart_cli();

extern void call_user(long start, long stack, int argc, char * argv[]);

/**
 * Initialize the procedure sub-system
 */
void proc_init() {
    /* Clear out the parameter buffer */
    g_proc_buffer_size = 0;
    g_proc_buffer = 0;
}

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

    INFO1("proc_exec start: %p", (void*)start);
    INFO1("proc_exec stack: %p", (void*)stack);

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
 * NOTE: this routine makes a local kernel copy of the parameters passed, but proc_exec does not
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

    TRACE1("proc_run(\"%s\")", path);

    /* TODO: allow for a search PATH */
    /* TODO: allocate stack more dynamically */

    int i;

    /* Figure out how much of a buffer that we need */
    int buffer_size = sizeof(int) * argc;
    for (i = 0; i < argc; i++) {
        buffer_size += strlen(argv[i]) + 1;
    }

    /* Return an old buffer, if it's too small */
    if ((g_proc_buffer_size > 0) && (g_proc_buffer_size < buffer_size)) {
        g_proc_buffer_size = 0;
        free(g_proc_buffer);
    }

    /* Create a new buffer if we don't have one */
    if (g_proc_buffer_size == 0) {
        g_proc_buffer_size = buffer_size;
        g_proc_buffer = (unsigned char *)malloc(buffer_size);
        if (g_proc_buffer == 0) {
            return ERR_OUT_OF_MEMORY;
        }
    }

    /* Pointer to our kernel copy of the argv array */
    char ** new_argv = g_proc_buffer;

    /* Actual strings start after the argv array */
    char * new_arg = g_proc_buffer + sizeof(int) * argc;

    /* Copy each argument into the kernel memory */
    for (i = 0; i < argc; i++) {
        new_argv[i] = new_arg;          /* Add the string to the array... */
        strcpy(new_arg, argv[i]);       /* Copy the actual characters to the kernel copy */
        new_arg += strlen(new_arg) + 1; /* And point to the next string (which should still be in the buffer) */
    }

    long start = 0;
    short result = fsys_load(path, 0, &start);
    if (result == 0) {
        if (start != 0) {
            proc_exec(start, k_default_stack, argc, new_argv);
            return 0;
        } else {
            ERROR1("Couldn't execute file: %d", result);
            return ERR_NOT_EXECUTABLE;
        }
    } else {
        ERROR1("Couldn't load file: %d", result);
        return result;
    }
}
