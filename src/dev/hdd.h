#ifndef HDD_H
#define HDD_H

#include <stdint.h>

//
// Install the PATA driver
//
// Returns:
//  0 on success, any negative number is an error code
//
 int16_t hdd_install();

#endif