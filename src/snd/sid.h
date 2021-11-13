/*
 * Definitions for the SID chips
 */

#ifndef __SID_H
#define __SID_H

/*
 * Reset all SID registers on the given SID chip
 *
 * Inputs:
 * sid = number of the SID to initialize (0 = Gideon mono, 1 = Gideon Left, 2 = Gideon Right, 3 = External Left, 4 = External Right)
 */
extern void sid_init(short sid);

/*
 * Initialize all the SID chips
 */
extern void sid_init_all();

/*
 * Test the internal SID implementation
 */
extern void sid_test_internal();

/*
 * Test the external SID implementation
 */
extern void sid_test_external();

#endif
