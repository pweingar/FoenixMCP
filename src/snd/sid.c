/*
 * Implementation of the SID code
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif

#include "features.h"
#include "sys_general.h"
#include "snd/sid.h"
#include "sound_reg.h"
#include "dev/rtc.h"

/*
 * Return the base address of the given SID chip
 *
 * Inputs:
 * sid = number of the SID to initialize (0 = Gideon mono, 1 = Gideon Left, 2 = Gideon Right, 3 = External Left, 4 = External Right)
 *
 * Returns:
 * address of the first SID register, 0 if the SID number is not valid
 */
volatile unsigned char * sid_get_base(short sid) {
    switch (sid) {
        case 0:
            return SID_INT_N_V1_FREQ_LO;
#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
        case 1:
            return SID_INT_L_V1_FREQ_LO;
        case 2:
            return SID_INT_R_V1_FREQ_LO;
        case 3:
            return SID_EXT_L_V1_FREQ_LO;
        case 4:
            return SID_EXT_R_V1_FREQ_LO;
#endif
        default:
            return 0;
    }
}

/*
 * Reset all SID registers on the given SID chip
 *
 * Inputs:
 * sid = number of the SID to initialize (0 = Gideon mono, 1 = Gideon Left, 2 = Gideon Right, 3 = External Left, 4 = External Right)
 */
void sid_init(short sid) {
    volatile unsigned char *sid_base = sid_get_base(sid);
    if (sid_base) {
        int offset;
        for (offset = 0; offset < 32; offset++) { // 32 is the number of register of a SID
            sid_base[offset] = 0;
        }
    }
}

/*
 * Initialize all the SID chips
 */
void sid_init_all() {
    int sid;
#if HAS_EXTERNAL_SIDS
    #define N_EXT_SIDS 2
#else
    #define N_EXT_SIDS 0
#endif
    for (sid = 0; sid <= 2+N_EXT_SIDS; sid++) {
        sid_init(sid);
    }

}

#if MODEL == MODEL_FOENIX_FMX || MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS
/*
 * Test the internal SID implementation
 */
void sid_test_internal() {
	volatile struct s_sid * sid = 0;

	sid = sid_get_base(0);
	if (sid)  {
		sid->v1.attack_decay = 0x29;
		sid->v2.attack_decay = 0x29;
		sid->v2.attack_decay = 0x29;

		sid->v1.sustain_release = 0x1f;
		sid->v2.sustain_release = 0x1f;
		sid->v3.sustain_release = 0x1f;

		sid->mode_volume = 0x0f;

		sid->v1.frequency = 0x1660;
		sid->v1.control = 0x11;

		// jiffies = rtc_get_jiffies() + 3;
    	// while (jiffies > rtc_get_jiffies());

		// sid->v2.frequency = 0x0831;
		// sid->v2.control = 0x11;

		// jiffies = rtc_get_jiffies() + 3;
    	// while (jiffies > rtc_get_jiffies());

		// sid->v3.frequency = 0x2187;
		// sid->v3.control = 0x11;

		// jiffies = rtc_get_jiffies() + 25;
		// while (jiffies > rtc_get_jiffies());

		// sid->v1.control = 0x10;
    	// jiffies = rtc_get_jiffies() + 3;
    	// while (jiffies > rtc_get_jiffies());

		// sid->v2.control = 0x10;
    	// jiffies = rtc_get_jiffies() + 3;
    	// while (jiffies > rtc_get_jiffies());

		// sid->v3.control = 0x10;

		// sid->mode_volume = 0;
	}
}

#else
/*
 * Test the internal SID implementation
 */
void sid_test_internal() {
    unsigned char i;
	unsigned int j;
    long jiffies;

	// Attack = 2, Decay = 9
	*SID_INT_L_V1_ATCK_DECY = 0x29;
	*SID_INT_L_V2_ATCK_DECY = 0x29;
	*SID_INT_L_V3_ATCK_DECY = 0x29;

	*SID_INT_R_V1_ATCK_DECY = 0x29;
	*SID_INT_R_V2_ATCK_DECY = 0x29;
	*SID_INT_R_V3_ATCK_DECY = 0x29;
	// Sustain = 1, Release = 5
	*SID_INT_L_V1_SSTN_RLSE = 0x1F;
	*SID_INT_L_V2_SSTN_RLSE = 0x1F;
	*SID_INT_L_V3_SSTN_RLSE = 0x1F;

	*SID_INT_R_V1_SSTN_RLSE = 0x1F;
	*SID_INT_R_V2_SSTN_RLSE = 0x1F;
	*SID_INT_R_V3_SSTN_RLSE = 0x1F;

	*SID_INT_L_MODE_VOL = 0x0F;
	*SID_INT_R_MODE_VOL = 0x0F;

	// Set Voice 1 to F-3
	*SID_INT_L_V1_FREQ_LO = 96;
	*SID_INT_L_V1_FREQ_HI = 22;
	*SID_INT_R_V1_FREQ_LO = 96;
	*SID_INT_R_V1_FREQ_HI = 22;

	*SID_INT_L_V1_CTRL = 0x11;
	*SID_INT_R_V1_CTRL = 0x11;

	jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_INT_L_V2_FREQ_LO = 49;
	*SID_INT_L_V2_FREQ_HI = 8;
	*SID_INT_R_V2_FREQ_LO = 49;
	*SID_INT_R_V2_FREQ_HI = 8;

	*SID_INT_L_V2_CTRL = 0x11;
	*SID_INT_R_V2_CTRL = 0x11;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_INT_L_V3_FREQ_LO = 135;
	*SID_INT_L_V3_FREQ_HI = 33;
	*SID_INT_R_V3_FREQ_LO = 135;
	*SID_INT_R_V3_FREQ_HI = 33;

	*SID_INT_L_V3_CTRL = 0x11;
	*SID_INT_R_V3_CTRL = 0x11;

    jiffies = rtc_get_jiffies() + 25;
    while (jiffies > rtc_get_jiffies());

	*SID_INT_L_V1_CTRL = 0x10;
	*SID_INT_R_V1_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_INT_L_V2_CTRL = 0x10;
	*SID_INT_R_V2_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_INT_L_V2_CTRL = 0x10;
	*SID_INT_R_V2_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 10;
    while (jiffies > rtc_get_jiffies());

	for (i = 0; i < 16; i++) {
		*SID_INT_L_MODE_VOL = 15 - i;
		*SID_INT_R_MODE_VOL = 15 - i;
	}

	*SID_INT_L_MODE_VOL = 0;
	*SID_INT_R_MODE_VOL = 0;
}
#endif

#if HAS_EXTERNAL_SIDS
void sid_test_external() {
    unsigned char i;
	unsigned int j;
    long jiffies;

	// Attack = 2, Decay = 9
	*SID_EXT_L_V1_ATCK_DECY = 0x29;
	*SID_EXT_L_V2_ATCK_DECY = 0x29;
	*SID_EXT_L_V3_ATCK_DECY = 0x29;

	*SID_EXT_R_V1_ATCK_DECY = 0x29;
	*SID_EXT_R_V2_ATCK_DECY = 0x29;
	*SID_EXT_R_V3_ATCK_DECY = 0x29;
	// Sustain = 1, Release = 5
	*SID_EXT_L_V1_SSTN_RLSE = 0x1F;
	*SID_EXT_L_V2_SSTN_RLSE = 0x1F;
	*SID_EXT_L_V3_SSTN_RLSE = 0x1F;

	*SID_EXT_R_V1_SSTN_RLSE = 0x1F;
	*SID_EXT_R_V2_SSTN_RLSE = 0x1F;
	*SID_EXT_R_V3_SSTN_RLSE = 0x1F;

	*SID_EXT_L_MODE_VOL = 0x0F;
	*SID_EXT_R_MODE_VOL = 0x0F;

	// Set Voice 1 to F-3
	*SID_EXT_L_V1_FREQ_LO = 96;
	*SID_EXT_L_V1_FREQ_HI = 22;
	*SID_EXT_R_V1_FREQ_LO = 96;
	*SID_EXT_R_V1_FREQ_HI = 22;

	*SID_EXT_L_V1_CTRL = 0x11;
	*SID_EXT_R_V1_CTRL = 0x11;

	jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_EXT_L_V2_FREQ_LO = 49;
	*SID_EXT_L_V2_FREQ_HI = 8;
	*SID_EXT_R_V2_FREQ_LO = 49;
	*SID_EXT_R_V2_FREQ_HI = 8;

	*SID_EXT_L_V2_CTRL = 0x11;
	*SID_EXT_R_V2_CTRL = 0x11;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_EXT_L_V3_FREQ_LO = 135;
	*SID_EXT_L_V3_FREQ_HI = 33;
	*SID_EXT_R_V3_FREQ_LO = 135;
	*SID_EXT_R_V3_FREQ_HI = 33;

	*SID_EXT_L_V3_CTRL = 0x11;
	*SID_EXT_R_V3_CTRL = 0x11;

    jiffies = rtc_get_jiffies() + 25;
    while (jiffies > rtc_get_jiffies());

	*SID_EXT_L_V1_CTRL = 0x10;
	*SID_EXT_R_V1_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_EXT_L_V2_CTRL = 0x10;
	*SID_EXT_R_V2_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 3;
    while (jiffies > rtc_get_jiffies());

	*SID_EXT_L_V2_CTRL = 0x10;
	*SID_EXT_R_V2_CTRL = 0x10;

    jiffies = rtc_get_jiffies() + 10;
    while (jiffies > rtc_get_jiffies());

	for (i = 0; i < 16; i++) {
		*SID_EXT_L_MODE_VOL = 15 - i;
		*SID_EXT_R_MODE_VOL = 15 - i;
	}

	*SID_EXT_L_MODE_VOL = 0;
	*SID_EXT_R_MODE_VOL = 0;
}
#endif /* HAS_EXTERNAL_SIDS */