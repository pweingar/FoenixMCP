/*
 * Processes
 */

#ifndef __PROC_H
#define __PROC_H

#define PROC_NAME_SIZE      8

#define PROC_STAT_RUNNING   0x01
#define PROC_STAT_IDLE      0x02

typedef struct s_proc_info {
    short status;                   /* The status of the process */
    short pid;                      /* The ID of the process */
    char name[PROC_NAME_SIZE];      /* A readable name for the process */
    int return_value;               /* The value returned by the process when it ended */
} t_proc_info, * p_proc_info;

/*
 * Initialize the process management system
 */
extern void proc_init();

/*
 * Terminate the current process with a return value
 *
 * Inputs:
 * return_value = the value to return from the process
 */
extern void proc_terminate(int return_value);

/*
 * Terminate a process with no return value.
 */
extern void proc_kill(short pid);

/*
 * Mark the process as running and make it the current process
 *
 * Mark the process that was current as IDLE
 */
extern void proc_running(short pid);

#endif