/*
 * Definitions for the OPL2
 */

#ifndef __OPL2_H
#define __OPL2_H

#define OPN2_OFFSET_PART_II		0x100

#define OPN2_22_LFO				0x22 //	LFO enable 	|	LFO frequency
#define OPN2_23_TIMER_A_H		0x24 //	Timer A MSBs
#define OPN2_24_TIMER_A_L		0x25 //	Timer A LSBs
#define OPN2_25_TIMER_B			0x26 //	Timer B
#define OPN2_27_CHANEL_3_MODE	0x27 //	Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
#define OPN2_27_TIMER_CONF		0x27 //	Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
#define OPN2_28_KEY_ON_OFF		0x28 //	Operator Channel
 //#define OPN2_29				0x29 // not used
#define OPN2_2A_ADC				0x2A //	DAC
#define OPN2_2B_ADC_EN			0x2B //	DAC en

//--------------------------------------------------------------------------------------
//------------------- DT1 (detune) and MUL (multiple) ----------------------------------
//--------------------------------------------------------------------------------------
//
// MUL ranges from 0 to 15 (decimal), and multiplies the overall frequency, with the
// exception that 0 results in multiplication by 1/2. That is, MUL=0 to 15 gives ×1/2,
// ×1, ×2, ... ×15.
//
// DT1 gives small variations from the overall frequency × MUL. The MSB of DT1 is a
// primitive sign bit, and the two LSB’s are magnitude bits. See the next page for a
// diagram.
//
//--------------------------------------------------------------------------------------

#define OPN2_30_ADSR__DT1_MUL__CH1_OP1		0x30 //	DT1 	MUL
#define OPN2_31_ADSR__DT1_MUL__CH2_OP1		0x31 //	DT1 	MUL
#define OPN2_32_ADSR__DT1_MUL__CH3_OP1		0x32 //	DT1 	MUL
#define OPN2_34_ADSR__DT1_MUL__CH1_OP2		0x34 //	DT1 	MUL
#define OPN2_35_ADSR__DT1_MUL__CH2_OP2		0x35 //	DT1 	MUL
#define OPN2_36_ADSR__DT1_MUL__CH3_OP2		0x36 //	DT1 	MUL
#define OPN2_38_ADSR__DT1_MUL__CH1_OP3		0x38 //	DT1 	MUL
#define OPN2_39_ADSR__DT1_MUL__CH2_OP3		0x39 //	DT1 	MUL
#define OPN2_3A_ADSR__DT1_MUL__CH3_OP3		0x3A //	DT1 	MUL
#define OPN2_3C_ADSR__DT1_MUL__CH1_OP4		0x3C //	DT1 	MUL
#define OPN2_3D_ADSR__DT1_MUL__CH2_OP4		0x3D //	DT1 	MUL
#define OPN2_3E_ADSR__DT1_MUL__CH3_OP4		0x3E //	DT1 	MUL
#define OPN2_30_ADSR__DT1_MUL__CH1_OP5		OPN2_OFFSET_PART_II + 0x30 //	DT1 	MUL
#define OPN2_31_ADSR__DT1_MUL__CH2_OP5		OPN2_OFFSET_PART_II + 0x31 //	DT1 	MUL
#define OPN2_32_ADSR__DT1_MUL__CH3_OP5		OPN2_OFFSET_PART_II + 0x32 //	DT1 	MUL
#define OPN2_34_ADSR__DT1_MUL__CH1_OP6		OPN2_OFFSET_PART_II + 0x34 //	DT1 	MUL
#define OPN2_35_ADSR__DT1_MUL__CH2_OP6		OPN2_OFFSET_PART_II + 0x35 //	DT1 	MUL
#define OPN2_36_ADSR__DT1_MUL__CH3_OP6		OPN2_OFFSET_PART_II + 0x36 //	DT1 	MUL
#define OPN2_38_ADSR__DT1_MUL__CH1_OP7		OPN2_OFFSET_PART_II + 0x38 //	DT1 	MUL
#define OPN2_39_ADSR__DT1_MUL__CH2_OP7		OPN2_OFFSET_PART_II + 0x39 //	DT1 	MUL
#define OPN2_3A_ADSR__DT1_MUL__CH3_OP7		OPN2_OFFSET_PART_II + 0x3A //	DT1 	MUL
#define OPN2_3C_ADSR__DT1_MUL__CH1_OP8		OPN2_OFFSET_PART_II + 0x3C //	DT1 	MUL
#define OPN2_3D_ADSR__DT1_MUL__CH2_OP8		OPN2_OFFSET_PART_II + 0x3D //	DT1 	MUL
#define OPN2_3E_ADSR__DT1_MUL__CH3_OP8		OPN2_OFFSET_PART_II + 0x3E //	DT1 	MUL

//--------------------------------------------------------------------------------------
//---------------------------------- TL (total level) ----------------------------------
//--------------------------------------------------------------------------------------
//
// TL (total level) represents the envelope’s highest amplitude, with 0 being the largest
// and 127 (decimal) the smallest. A change of one unit is about 0.75 dB.
//
// To make a note softer, only change the TL of the slots (the output operators).
// Changing the other operators will affect the flavor of the note.
//
//--------------------------------------------------------------------------------------

#define OPN2_40_ADSR__LT__CH1_OP1		0x40 //	LT
#define OPN2_41_ADSR__LT__CH2_OP1		0x41 //	LT
#define OPN2_42_ADSR__LT__CH3_OP1		0x42 //	LT
#define OPN2_44_ADSR__LT__CH1_OP2		0x44 //	LT
#define OPN2_45_ADSR__LT__CH2_OP2		0x45 //	LT
#define OPN2_46_ADSR__LT__CH3_OP2		0x46 //	LT
#define OPN2_48_ADSR__LT__CH1_OP3		0x48 //	LT
#define OPN2_49_ADSR__LT__CH2_OP3		0x49 //	LT
#define OPN2_4A_ADSR__LT__CH3_OP3		0x4A //	LT
#define OPN2_4C_ADSR__LT__CH1_OP4		0x4C //	LT
#define OPN2_4D_ADSR__LT__CH2_OP4		0x4D //	LT
#define OPN2_4E_ADSR__LT__CH3_OP4		0x4E //	LT
#define OPN2_40_ADSR__LT__CH1_OP5		OPN2_OFFSET_PART_II + 0x40 //	LT
#define OPN2_41_ADSR__LT__CH2_OP5		OPN2_OFFSET_PART_II + 0x41 //	LT
#define OPN2_42_ADSR__LT__CH3_OP5		OPN2_OFFSET_PART_II + 0x42 //	LT
#define OPN2_44_ADSR__LT__CH1_OP6		OPN2_OFFSET_PART_II + 0x44 //	LT
#define OPN2_45_ADSR__LT__CH2_OP6		OPN2_OFFSET_PART_II + 0x45 //	LT
#define OPN2_46_ADSR__LT__CH3_OP6		OPN2_OFFSET_PART_II + 0x46 //	LT
#define OPN2_48_ADSR__LT__CH1_OP7		OPN2_OFFSET_PART_II + 0x48 //	LT
#define OPN2_49_ADSR__LT__CH2_OP7		OPN2_OFFSET_PART_II + 0x49 //	LT
#define OPN2_4A_ADSR__LT__CH3_OP7		OPN2_OFFSET_PART_II + 0x4A //	LT
#define OPN2_4C_ADSR__LT__CH1_OP8		OPN2_OFFSET_PART_II + 0x4C //	LT
#define OPN2_4D_ADSR__LT__CH2_OP8		OPN2_OFFSET_PART_II + 0x4D //	LT
#define OPN2_4E_ADSR__LT__CH3_OP8		OPN2_OFFSET_PART_II + 0x4E //	LT

//--------------------------------------------------------------------------------------
//------------------- RS (rate scaling) and AR (attack rate) ---------------------------
//--------------------------------------------------------------------------------------
//
// AR is the steepness of the initial amplitude rise, shown on page 5.
//
// RS affects AR, D1R, D2R and RR in the same way. RS is the degree to which the envelope
// becomes narrower as the frequency becomes higher.
//
//--------------------------------------------------------------------------------------

#define OPN2_50_ADSR__SR_AR__CH1_OP1		0x50 //	SR AR
#define OPN2_51_ADSR__SR_AR__CH2_OP1		0x51 //	SR AR
#define OPN2_52_ADSR__SR_AR__CH3_OP1		0x52 //	SR AR
#define OPN2_54_ADSR__SR_AR__CH1_OP2		0x54 //	SR AR
#define OPN2_55_ADSR__SR_AR__CH2_OP2		0x55 //	SR AR
#define OPN2_56_ADSR__SR_AR__CH3_OP2		0x56 //	SR AR
#define OPN2_58_ADSR__SR_AR__CH1_OP3		0x58 //	SR AR
#define OPN2_59_ADSR__SR_AR__CH2_OP3		0x59 //	SR AR
#define OPN2_5A_ADSR__SR_AR__CH3_OP3		0x5A //	SR AR
#define OPN2_5C_ADSR__SR_AR__CH1_OP4		0x5C //	SR AR
#define OPN2_5D_ADSR__SR_AR__CH2_OP4		0x5D //	SR AR
#define OPN2_5E_ADSR__SR_AR__CH3_OP4		0x5E //	SR AR
#define OPN2_50_ADSR__SR_AR__CH1_OP5		OPN2_OFFSET_PART_II + 0x50 //	SR AR
#define OPN2_51_ADSR__SR_AR__CH2_OP5		OPN2_OFFSET_PART_II + 0x51 //	SR AR
#define OPN2_52_ADSR__SR_AR__CH3_OP5		OPN2_OFFSET_PART_II + 0x52 //	SR AR
#define OPN2_54_ADSR__SR_AR__CH1_OP6		OPN2_OFFSET_PART_II + 0x54 //	SR AR
#define OPN2_55_ADSR__SR_AR__CH2_OP6		OPN2_OFFSET_PART_II + 0x55 //	SR AR
#define OPN2_56_ADSR__SR_AR__CH3_OP6		OPN2_OFFSET_PART_II + 0x56 //	SR AR
#define OPN2_58_ADSR__SR_AR__CH1_OP7		OPN2_OFFSET_PART_II + 0x58 //	SR AR
#define OPN2_59_ADSR__SR_AR__CH2_OP7		OPN2_OFFSET_PART_II + 0x59 //	SR AR
#define OPN2_5A_ADSR__SR_AR__CH3_OP7		OPN2_OFFSET_PART_II + 0x5A //	SR AR
#define OPN2_5C_ADSR__SR_AR__CH1_OP8		OPN2_OFFSET_PART_II + 0x5C //	SR AR
#define OPN2_5D_ADSR__SR_AR__CH2_OP8		OPN2_OFFSET_PART_II + 0x5D //	SR AR
#define OPN2_5E_ADSR__SR_AR__CH3_OP8		OPN2_OFFSET_PART_II + 0x5E //	SR AR

//--------------------------------------------------------------------------------------
//-------------- AM modulation enable amd D1R (first decay rate) -----------------------
//--------------------------------------------------------------------------------------
//
// D1R (first decay rate) is the initial steep amplitude decay rate (see page 4). It is,
// like all rates, 0-31 in value and affected by RS.
//
// AM is the amplitude modulation enable, whether of not this operator will be subject to
// amplitude modulation by the LFO. This bit is not relevant unless both the LFO is
// enabled and register B4′s AMS (amplitude modulation sensitivity) is non-zero.
//
//--------------------------------------------------------------------------------------

#define OPN2_60_ADSR__AM_D1R__CH1_OP1		0x60 //	AM D1R
#define OPN2_61_ADSR__AM_D1R__CH2_OP1		0x61 //	AM D1R
#define OPN2_62_ADSR__AM_D1R__CH3_OP1		0x62 //	AM D1R
#define OPN2_64_ADSR__AM_D1R__CH1_OP2		0x64 //	AM D1R
#define OPN2_65_ADSR__AM_D1R__CH2_OP2		0x65 //	AM D1R
#define OPN2_66_ADSR__AM_D1R__CH3_OP2		0x66 //	AM D1R
#define OPN2_68_ADSR__AM_D1R__CH1_OP3		0x68 //	AM D1R
#define OPN2_69_ADSR__AM_D1R__CH2_OP3		0x69 //	AM D1R
#define OPN2_6A_ADSR__AM_D1R__CH3_OP3		0x6A //	AM D1R
#define OPN2_6C_ADSR__AM_D1R__CH1_OP4		0x6C //	AM D1R
#define OPN2_6D_ADSR__AM_D1R__CH2_OP4		0x6D //	AM D1R
#define OPN2_6E_ADSR__AM_D1R__CH3_OP4		0x6E //	AM D1R
#define OPN2_60_ADSR__AM_D1R__CH1_OP5		OPN2_OFFSET_PART_II + 0x60 //	AM D1R
#define OPN2_61_ADSR__AM_D1R__CH2_OP5		OPN2_OFFSET_PART_II + 0x61 //	AM D1R
#define OPN2_62_ADSR__AM_D1R__CH3_OP5		OPN2_OFFSET_PART_II + 0x62 //	AM D1R
#define OPN2_64_ADSR__AM_D1R__CH1_OP6		OPN2_OFFSET_PART_II + 0x64 //	AM D1R
#define OPN2_65_ADSR__AM_D1R__CH2_OP6		OPN2_OFFSET_PART_II + 0x65 //	AM D1R
#define OPN2_66_ADSR__AM_D1R__CH3_OP6		OPN2_OFFSET_PART_II + 0x66 //	AM D1R
#define OPN2_68_ADSR__AM_D1R__CH1_OP7		OPN2_OFFSET_PART_II + 0x68 //	AM D1R
#define OPN2_69_ADSR__AM_D1R__CH2_OP7		OPN2_OFFSET_PART_II + 0x69 //	AM D1R
#define OPN2_6A_ADSR__AM_D1R__CH3_OP7		OPN2_OFFSET_PART_II + 0x6A //	AM D1R
#define OPN2_6C_ADSR__AM_D1R__CH1_OP8		OPN2_OFFSET_PART_II + 0x6C //	AM D1R
#define OPN2_6D_ADSR__AM_D1R__CH2_OP8		OPN2_OFFSET_PART_II + 0x6D //	AM D1R
#define OPN2_6E_ADSR__AM_D1R__CH3_OP8		OPN2_OFFSET_PART_II + 0x6E //	AM D1R

//--------------------------------------------------------------------------------------
//-------------------------- D2R (secondary decay rate) --------------------------------
//--------------------------------------------------------------------------------------
//
// D2R (secondary decay rate) is the long tail off of the sound that continues as long
// as the key is depressed.
//
//--------------------------------------------------------------------------------------

#define OPN2_70_ADSR__D2R__CH1_OP1		0x70 //	D2R
#define OPN2_71_ADSR__D2R__CH2_OP1		0x71 //	D2R
#define OPN2_72_ADSR__D2R__CH3_OP1		0x72 //	D2R
#define OPN2_74_ADSR__D2R__CH1_OP2		0x74 //	D2R
#define OPN2_75_ADSR__D2R__CH2_OP2		0x75 //	D2R
#define OPN2_76_ADSR__D2R__CH3_OP2		0x76 //	D2R
#define OPN2_78_ADSR__D2R__CH1_OP3		0x78 //	D2R
#define OPN2_79_ADSR__D2R__CH2_OP3		0x79 //	D2R
#define OPN2_7A_ADSR__D2R__CH3_OP3		0x7A //	D2R
#define OPN2_7C_ADSR__D2R__CH1_OP4		0x7C //	D2R
#define OPN2_7D_ADSR__D2R__CH2_OP4		0x7D //	D2R
#define OPN2_7E_ADSR__D2R__CH3_OP4		0x7E //	D2R
#define OPN2_70_ADSR__D2R__CH1_OP5		OPN2_OFFSET_PART_II + 0x70 //	D2R
#define OPN2_71_ADSR__D2R__CH2_OP5		OPN2_OFFSET_PART_II + 0x71 //	D2R
#define OPN2_72_ADSR__D2R__CH3_OP5		OPN2_OFFSET_PART_II + 0x72 //	D2R
#define OPN2_74_ADSR__D2R__CH1_OP6		OPN2_OFFSET_PART_II + 0x74 //	D2R
#define OPN2_75_ADSR__D2R__CH2_OP6		OPN2_OFFSET_PART_II + 0x75 //	D2R
#define OPN2_76_ADSR__D2R__CH3_OP6		OPN2_OFFSET_PART_II + 0x76 //	D2R
#define OPN2_78_ADSR__D2R__CH1_OP7		OPN2_OFFSET_PART_II + 0x78 //	D2R
#define OPN2_79_ADSR__D2R__CH2_OP7		OPN2_OFFSET_PART_II + 0x79 //	D2R
#define OPN2_7A_ADSR__D2R__CH3_OP7		OPN2_OFFSET_PART_II + 0x7A //	D2R
#define OPN2_7C_ADSR__D2R__CH1_OP8		OPN2_OFFSET_PART_II + 0x7C //	D2R
#define OPN2_7D_ADSR__D2R__CH2_OP8		OPN2_OFFSET_PART_II + 0x7D //	D2R
#define OPN2_7E_ADSR__D2R__CH3_OP8		OPN2_OFFSET_PART_II + 0x7E //	D2R

//--------------------------------------------------------------------------------------
//-------------------------- D2R (secondary decay rate) --------------------------------
//--------------------------------------------------------------------------------------
//
// D2R (secondary decay rate) is the long tail off of the sound that continues as long
// as the key is depressed.
//
//--------------------------------------------------------------------------------------

#define OPN2_80_ADSR__D1L_RR__CH1_OP1		0x80 //	D1L RR
#define OPN2_81_ADSR__D1L_RR__CH2_OP1		0x81 //	D1L RR
#define OPN2_82_ADSR__D1L_RR__CH3_OP1		0x82 //	D1L RR
#define OPN2_84_ADSR__D1L_RR__CH1_OP2		0x84 //	D1L RR
#define OPN2_85_ADSR__D1L_RR__CH2_OP2		0x85 //	D1L RR
#define OPN2_86_ADSR__D1L_RR__CH3_OP2		0x86 //	D1L RR
#define OPN2_88_ADSR__D1L_RR__CH1_OP3		0x88 //	D1L RR
#define OPN2_89_ADSR__D1L_RR__CH2_OP3		0x89 //	D1L RR
#define OPN2_8A_ADSR__D1L_RR__CH3_OP3		0x8A //	D1L RR
#define OPN2_8C_ADSR__D1L_RR__CH1_OP4		0x8C //	D1L RR
#define OPN2_8D_ADSR__D1L_RR__CH2_OP4		0x8D //	D1L RR
#define OPN2_8E_ADSR__D1L_RR__CH3_OP4		0x8E //	D1L RR
#define OPN2_80_ADSR__D1L_RR__CH1_OP5		OPN2_OFFSET_PART_II + 0x80 //	D1L RR
#define OPN2_81_ADSR__D1L_RR__CH2_OP5		OPN2_OFFSET_PART_II + 0x81 //	D1L RR
#define OPN2_82_ADSR__D1L_RR__CH3_OP5		OPN2_OFFSET_PART_II + 0x82 //	D1L RR
#define OPN2_84_ADSR__D1L_RR__CH1_OP6		OPN2_OFFSET_PART_II + 0x84 //	D1L RR
#define OPN2_85_ADSR__D1L_RR__CH2_OP6		OPN2_OFFSET_PART_II + 0x85 //	D1L RR
#define OPN2_86_ADSR__D1L_RR__CH3_OP6		OPN2_OFFSET_PART_II + 0x86 //	D1L RR
#define OPN2_88_ADSR__D1L_RR__CH1_OP7		OPN2_OFFSET_PART_II + 0x88 //	D1L RR
#define OPN2_89_ADSR__D1L_RR__CH2_OP7		OPN2_OFFSET_PART_II + 0x89 //	D1L RR
#define OPN2_8A_ADSR__D1L_RR__CH3_OP7		OPN2_OFFSET_PART_II + 0x8A //	D1L RR
#define OPN2_8C_ADSR__D1L_RR__CH1_OP8		OPN2_OFFSET_PART_II + 0x8C //	D1L RR
#define OPN2_8D_ADSR__D1L_RR__CH2_OP8		OPN2_OFFSET_PART_II + 0x8D //	D1L RR
#define OPN2_8E_ADSR__D1L_RR__CH3_OP8		OPN2_OFFSET_PART_II + 0x8E //	D1L RR

//--------------------------------------------------------------------------------------
//------------------------------------ SSG-EG ------------------------------------------
//--------------------------------------------------------------------------------------
//
// This register is proprietary and should be set to zero.
//
//--------------------------------------------------------------------------------------

#define OPN2_90_ADSR__D1L_RR__CH1_OP1		0x90 //	D1L RR
#define OPN2_91_ADSR__D1L_RR__CH2_OP1		0x91 //	D1L RR
#define OPN2_92_ADSR__D1L_RR__CH3_OP1		0x92 //	D1L RR
#define OPN2_94_ADSR__D1L_RR__CH1_OP2		0x94 //	D1L RR
#define OPN2_95_ADSR__D1L_RR__CH2_OP2		0x95 //	D1L RR
#define OPN2_96_ADSR__D1L_RR__CH3_OP2		0x96 //	D1L RR
#define OPN2_98_ADSR__D1L_RR__CH1_OP3		0x98 //	D1L RR
#define OPN2_99_ADSR__D1L_RR__CH2_OP3		0x99 //	D1L RR
#define OPN2_9A_ADSR__D1L_RR__CH3_OP3		0x9A //	D1L RR
#define OPN2_9C_ADSR__D1L_RR__CH1_OP4		0x9C //	D1L RR
#define OPN2_9D_ADSR__D1L_RR__CH2_OP4		0x9D //	D1L RR
#define OPN2_9E_ADSR__D1L_RR__CH3_OP4		0x9E //	D1L RR
#define OPN2_90_ADSR__D1L_RR__CH4_OP1		OPN2_OFFSET_PART_II + 0x90 //	D1L RR
#define OPN2_91_ADSR__D1L_RR__CH5_OP1		OPN2_OFFSET_PART_II + 0x91 //	D1L RR
#define OPN2_92_ADSR__D1L_RR__CH6_OP1		OPN2_OFFSET_PART_II + 0x92 //	D1L RR
#define OPN2_94_ADSR__D1L_RR__CH4_OP2		OPN2_OFFSET_PART_II + 0x94 //	D1L RR
#define OPN2_95_ADSR__D1L_RR__CH5_OP2		OPN2_OFFSET_PART_II + 0x95 //	D1L RR
#define OPN2_96_ADSR__D1L_RR__CH6_OP2		OPN2_OFFSET_PART_II + 0x96 //	D1L RR
#define OPN2_98_ADSR__D1L_RR__CH4_OP3		OPN2_OFFSET_PART_II + 0x98 //	D1L RR
#define OPN2_99_ADSR__D1L_RR__CH5_OP3		OPN2_OFFSET_PART_II + 0x99 //	D1L RR
#define OPN2_9A_ADSR__D1L_RR__CH6_OP3		OPN2_OFFSET_PART_II + 0x9A //	D1L RR
#define OPN2_9C_ADSR__D1L_RR__CH4_OP4		OPN2_OFFSET_PART_II + 0x9C //	D1L RR
#define OPN2_9D_ADSR__D1L_RR__CH5_OP4		OPN2_OFFSET_PART_II + 0x9D //	D1L RR
#define OPN2_9E_ADSR__D1L_RR__CH6_OP4		OPN2_OFFSET_PART_II + 0x9E //	D1L RR

#define OPN2_A0_CH1_FRECANCY_L				0xA0 //	Frequency number LSB
#define OPN2_A1_CH2_FRECANCY_L				0xA1 //	Frequency number LSB
#define OPN2_A2_CH3_FRECANCY_L				0xA2 //	Frequency number LSB
#define OPN2_A2_CH3_FRECANCY_OP1_L			0xA2 //	Frequency number LSB Chanel 3 Operator 1 if special mode
#define OPN2_A8_CH3_FRECANCY_OP2_L			0xA8 //	Frequency number LSB Chanel 3 Operator 2 if special mode
#define OPN2_A9_CH3_FRECANCY_OP3_L			0xA9 //	Frequency number LSB Chanel 3 Operator 3 if special mode
#define OPN2_AA_CH3_FRECANCY_OP4_L			0xAA //	Frequency number LSB Chanel 3 Operator 4 if special mode
#define OPN2_A0_CH4_FRECANCY_L				OPN2_OFFSET_PART_II + 0xA0 //	Frequency number LSB
#define OPN2_A1_CH5_FRECANCY_L				OPN2_OFFSET_PART_II + 0xA1 //	Frequency number LSB
#define OPN2_A2_CH6_FRECANCY_L				OPN2_OFFSET_PART_II + 0xA2 //	Frequency number LSB

#define OPN2_A4_CH1_OCTAVE_FRECANCY_H		0xA4 //	OCTAVE and Frequency number MSB
#define OPN2_A5_CH2_OCTAVE_FRECANCY_H		0xA5 //	OCTAVE and Frequency number MSB
#define OPN2_A6_CH3_OCTAVE_FRECANCY_H		0xA6 //	OCTAVE and Frequency number MSB
#define OPN2_A6_CH3_OCTAVE_FRECANCY_OP1_H	0xA6 //	OCTAVE and Frequency number MSB Chanel 3 Operator 1 if special mode
#define OPN2_AC_CH3_OCTAVE_FRECANCY_OP2_H	0xAC //	OCTAVE and Frequency number MSB Chanel 3 Operator 2 if special mode
#define OPN2_AD_CH3_OCTAVE_FRECANCY_OP3_H	0xAD //	OCTAVE and Frequency number MSB Chanel 3 Operator 3 if special mode
#define OPN2_AE_CH3_OCTAVE_FRECANCY_OP4_H	0xAE //	OCTAVE and Frequency number MSB Chanel 3 Operator 4 if special mode
#define OPN2_A4_CH4_OCTAVE_FRECANCY_H		OPN2_OFFSET_PART_II + 0xA4 //	OCTAVE and Frequency number MSB
#define OPN2_A5_CH5_OCTAVE_FRECANCY_H		OPN2_OFFSET_PART_II + 0xA5 //	OCTAVE and Frequency number MSB
#define OPN2_A6_CH6_OCTAVE_FRECANCY_H		OPN2_OFFSET_PART_II + 0xA6 //	OCTAVE and Frequency number MSB

// Feedback is the degree to which operator 1 feeds back into itself.
// The algorithm is the type of inter-operator connection used

#define OPN2_B0_CH1_FEEDBACK_ALGO			0xB0 //
#define OPN2_B1_CH2_FEEDBACK_ALGO			0xB1 //
#define OPN2_B2_CH3_FEEDBACK_ALGO			0xB2 //
#define OPN2_B0_CH4_FEEDBACK_ALGO			OPN2_OFFSET_PART_II + 0xB0 //
#define OPN2_B1_CH5_FEEDBACK_ALGO			OPN2_OFFSET_PART_II + 0xB1 //
#define OPN2_B2_CH6_FEEDBACK_ALGO			OPN2_OFFSET_PART_II + 0xB2 //

// Register B4H contains stereo output control and LFO sensitivity control.

#define OPN2_B4_CH1_L_R_AMS_FMS				0xB4 //
#define OPN2_B5_CH2_L_R_AMS_FMS				0xB5 //
#define OPN2_B6_CH3_L_R_AMS_FMS				0xB6 //
#define OPN2_B4_CH4_L_R_AMS_FMS				OPN2_OFFSET_PART_II + 0xB4 //
#define OPN2_B5_CH5_L_R_AMS_FMS				OPN2_OFFSET_PART_II + 0xB5 //
#define OPN2_B6_CH6_L_R_AMS_FMS				OPN2_OFFSET_PART_II + 0xB6 //

#endif
