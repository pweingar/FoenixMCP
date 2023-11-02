/**
 * @file gabe.h
 * @brief Definitions for the GABE chip
 * @version 0.1
 * @date 2023-08-30
 * 
 */

#ifndef __GABE_H_
#define __GABE_H_

#include <stdint.h>
#include "types.h"

#define GABE_CTRL_REG			((volatile __attribute__((far)) uint8_t *)0xafe880)
#define GABE_CTRL_PWR_LED		0x01     	// Controls the LED in the Front of the case (Next to the reset button)
#define GABE_CTRL_SDC_LED		0x02     	// Controls the LED in the Front of the Case (Next to SDCard)
#define GABE_CTRL_STS_LED0		0x04     	// Control Status LED0 (General Use) - C256 Foenix U Only
#define GABE_CTRL_STS_LED1		0x08     	// Control Status LED0 (General Use) - C256 Foenix U Only
#define BUZZER_CONTROL			0x10     	// Controls the Buzzer
#define GABE_CTRL_WRM_RST		0x80    	// Warm Reset (needs to Setup other registers)

#define GABE_RST_AUTH			((volatile __attribute__((far)) uint16_t *)0xafe882)	// Set to 0xDEAD to enable reset


#define GABE_LED_FLASH_CTRL		((volatile __attribute__((far)) uint8_t *)0xafe881)		// Flashing LED Control
#define GABE_LED0_FLASH_CTRL	0x01   		// 0- Automatic Flash 1 - Bypass Flash Timer (Use GABE_CTRL_STS_LED0 to manually control)
#define GABE_LED1_FLASH_CTRL	0x02  		// 0- Automatic Flash 1 - Bypass Flash Timer (Use GABE_CTRL_STS_LED1 to manually control)

#define GABE_LD0_FLASH_FRQ_MASK	0x30
#define GABE_LD0_FLASH_FRQ_1HZ	0x00		// LD0 flashes at 1Hz
#define GABE_LD0_FLASH_FRQ_2HZ	0x10		// LD0 flashes at 2Hz
#define GABE_LD0_FLASH_FRQ_4HZ	0x20		// LD0 flashes at 4Hz
#define GABE_LD0_FLASH_FRQ_5HZ	0x30		// LD0 flashes at 5Hz

#define GABE_LD1_FLASH_FRQ_MASK	0xc0
#define GABE_LD1_FLASH_FRQ_1HZ	0x00		// LD1 flashes at 2Hz
#define GABE_LD1_FLASH_FRQ_2HZ	0x40		// LD1 flashes at 4Hz
#define GABE_LD1_FLASH_FRQ_4HZ	0x80		// LD1 flashes at 5Hz
#define GABE_LD1_FLASH_FRQ_5HZ	0xc0		// LD1 flashes at 1Hz

#define GABE_RNG_DATA			((volatile __attribute__((far)) uint16_t *)0xafe884)	// Random Number Generator (read for data, write for seed)
#define GABE_RNG_SEED			((volatile __attribute__((far)) uint16_t *)0xafe884)	// Random Number Generator (read for data, write for seed)
#define GABE_RNG_STAT			((volatile __attribute__((far)) uint16_t *)0xafe886)	// Random Number Generator Status (read)
#define GABE_RNG_LFSR_DONE		0x80
#define GABE_RNG_CTRL			((volatile __attribute__((far)) uint16_t *)0xafe886)	// Random Number Generator Control (write)
#define GABE_RNG_CTRL_EN		0x01													// Enable the LFSR block
#define GABE_RNG_CTRL_LD_SEED	0x02													// Toggle after setting seed to load seed

#define GABE_DIP_REG			((volatile __attribute__((far)) uint16_t *)0xafe80e)	// User and boot mode DIP switches
#define HD_INSTALLED 			0x0080
#define DIP_BOOT_IDE			0x0000
#define DIP_BOOT_SDCARD			0x0100
#define DIP_BOOT_FLOPPY			0x0200

/**
 * @brief Structure to represent the machine ID and expansion card info
 * 
 */
union gabe_sys_stat_u {
	struct {
		uint8_t machine_id:3;
		uint8_t rsrv:1;
		uint8_t no_expansion:1;
	};
	uint8_t reg;
};

#define GABE_SYS_STAT			((volatile __attribute__((far)) union gabe_sys_stat_u *)0xafe887)

/**
 * @brief Structure to respresent the version of the GABE chip
 * 
 */
struct gabe_version_s {
	uint16_t subversion;
	uint16_t version;
	uint16_t model;
};

#define GABE_VERSION			((volatile __attribute__((far)) struct gabe_version_s *)0xafe88a)

#endif

