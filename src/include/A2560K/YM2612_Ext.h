#ifndef YM2612_Ext_H_
#define YM2612_Ext_H_


// External Registers $00C20400..$00C20BFF
#define EXT_OPN2_22_LFO 			((uint8_t* const)0xFEC20422) // LFO enable 	|	LFO frequency
#define EXT_OPN2_23_TIMER_A_H 	((uint8_t* const)0xFEC20423) // Timer A MSBs
#define EXT_OPN2_24_TIMER_A_L 	((uint8_t* const)0xFEC20424) // Timer A LSBs
#define EXT_OPN2_25_TIMER_B 		((uint8_t* const)0xFEC20425) // Timer B
#define EXT_OPN2_27_CHANEL_3_MODE ((uint8_t* const)0xFEC20427) // Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
#define EXT_OPN2_27_TIMER_CONF 	((uint8_t* const)0xFEC20427) // Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
#define EXT_OPN2_28_KEY_ON_OFF 	((uint8_t* const)0xFEC20428) // Operator Channel
#define EXT_OPN2_29					((uint8_t* const)0xFEC20429)
#define EXT_OPN2_2A_ADC 			((uint8_t* const)0xFEC2042A) // DAC
#define EXT_OPN2_2B_ADC_EN 		((uint8_t* const)0xFEC2042B) // DAC en
//)--------------------------------------------------------------------------------------
//)------------------- DT1 (detune) and MUL (multiple) ----------------------------------
//)--------------------------------------------------------------------------------------
//)
//) MUL ranges from 0 to 15 (decimal), and multiplies the overall frequency, with the
//) exception that 0 results in multiplication by 1/2. That is, MUL=0 to 15 gives ×1/2,
//) ×1, ×2, ... ×15.
//)
//) DT1 gives small variations from the overall frequency × MUL. The MSB of DT1 is a
//) primitive sign bit, and the two LSB’s are magnitude bits. See the next page for a
//) diagram.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP1		((uint8_t* const) 0xFEC20430)
#define EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP1		((uint8_t* const) 0xFEC20431)
#define EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP1		((uint8_t* const) 0xFEC20432)
#define EXT_OPN2_34_ADSR__DT1_MUL__CH1_OP2		((uint8_t* const) 0xFEC20434)
#define EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP2		((uint8_t* const) 0xFEC20435)
#define EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP2		((uint8_t* const) 0xFEC20436)
#define EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP3		((uint8_t* const) 0xFEC20438)
#define EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP3		((uint8_t* const) 0xFEC20439)
#define EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP3		((uint8_t* const) 0xFEC2043A)
#define EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP4		((uint8_t* const) 0xFEC2043C)
#define EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP4		((uint8_t* const) 0xFEC2043D)
#define EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP4		((uint8_t* const) 0xFEC2043E)
#define EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP5		((uint8_t* const) 0xFEC20530)
#define EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP5		((uint8_t* const) 0xFEC20531)
#define EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP5		((uint8_t* const) 0xFEC20532)
#define EXT_OPN2_34_ADSR__DT1_MUL__CH1_OP6		((uint8_t* const) 0xFEC20534)
#define EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP6		((uint8_t* const) 0xFEC20535)
#define EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP6		((uint8_t* const) 0xFEC20536)
#define EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP7		((uint8_t* const) 0xFEC20538)
#define EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP7		((uint8_t* const) 0xFEC20539)
#define EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP7		((uint8_t* const) 0xFEC2053A)
#define EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP8		((uint8_t* const) 0xFEC2053C)
#define EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP8		((uint8_t* const) 0xFEC2053D)
#define EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP8		((uint8_t* const) 0xFEC2053E)

//)--------------------------------------------------------------------------------------
//)---------------------------------- TL (total level) ----------------------------------
//)--------------------------------------------------------------------------------------
//)
//) TL (total level) represents the envelope’s highest amplitude, with 0 being the largest
//) and 127 (decimal) the smallest. A change of one unit is about 0.75 dB.
//)
//) To make a note softer, only change the TL of the slots (the output operators).
//) Changing the other operators will affect the flavor of the note.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_40_ADSR__LT__CH1_OP1		((uint8_t* const) 0xFEC20440)
#define EXT_OPN2_41_ADSR__LT__CH2_OP1		((uint8_t* const) 0xFEC20441)
#define EXT_OPN2_42_ADSR__LT__CH3_OP1		((uint8_t* const) 0xFEC20442)
#define EXT_OPN2_44_ADSR__LT__CH1_OP2		((uint8_t* const) 0xFEC20444)
#define EXT_OPN2_45_ADSR__LT__CH2_OP2		((uint8_t* const) 0xFEC20445)
#define EXT_OPN2_46_ADSR__LT__CH3_OP2		((uint8_t* const) 0xFEC20446)
#define EXT_OPN2_48_ADSR__LT__CH1_OP3		((uint8_t* const) 0xFEC20448)
#define EXT_OPN2_49_ADSR__LT__CH2_OP3		((uint8_t* const) 0xFEC20449)
#define EXT_OPN2_4A_ADSR__LT__CH3_OP3		((uint8_t* const) 0xFEC2044A)
#define EXT_OPN2_4C_ADSR__LT__CH1_OP4		((uint8_t* const) 0xFEC2044C)
#define EXT_OPN2_4D_ADSR__LT__CH2_OP4		((uint8_t* const) 0xFEC2044D)
#define EXT_OPN2_4E_ADSR__LT__CH3_OP4		((uint8_t* const) 0xFEC2044E)
#define EXT_OPN2_40_ADSR__LT__CH1_OP5		((uint8_t* const) 0xFEC20540)
#define EXT_OPN2_41_ADSR__LT__CH2_OP5		((uint8_t* const) 0xFEC20541)
#define EXT_OPN2_42_ADSR__LT__CH3_OP5		((uint8_t* const) 0xFEC20542)
#define EXT_OPN2_44_ADSR__LT__CH1_OP6		((uint8_t* const) 0xFEC20544)
#define EXT_OPN2_45_ADSR__LT__CH2_OP6		((uint8_t* const) 0xFEC20545)
#define EXT_OPN2_46_ADSR__LT__CH3_OP6		((uint8_t* const) 0xFEC20546)
#define EXT_OPN2_48_ADSR__LT__CH1_OP7		((uint8_t* const) 0xFEC20548)
#define EXT_OPN2_49_ADSR__LT__CH2_OP7		((uint8_t* const) 0xFEC20549)
#define EXT_OPN2_4A_ADSR__LT__CH3_OP7		((uint8_t* const) 0xFEC2054A)
#define EXT_OPN2_4C_ADSR__LT__CH1_OP8		((uint8_t* const) 0xFEC2054C)
#define EXT_OPN2_4D_ADSR__LT__CH2_OP8		((uint8_t* const) 0xFEC2054D)
#define EXT_OPN2_4E_ADSR__LT__CH3_OP8		((uint8_t* const) 0xFEC2054E)
//)--------------------------------------------------------------------------------------
//)------------------- RS (rate scaling) and AR (attack rate) ---------------------------
//)--------------------------------------------------------------------------------------
//)
//) AR is the steepness of the initial amplitude rise, shown on page 5.
//)
//) RS affects AR, D1R, D2R and RR in the same way. RS is the degree to which the envelope
//) becomes narrower as the frequency becomes higher.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_50_ADSR__SR_AR__CH1_OP1		((uint8_t* const) 0xFEC20450)
#define EXT_OPN2_51_ADSR__SR_AR__CH2_OP1		((uint8_t* const) 0xFEC20451)
#define EXT_OPN2_52_ADSR__SR_AR__CH3_OP1		((uint8_t* const) 0xFEC20452)
#define EXT_OPN2_54_ADSR__SR_AR__CH1_OP2		((uint8_t* const) 0xFEC20454)
#define EXT_OPN2_55_ADSR__SR_AR__CH2_OP2		((uint8_t* const) 0xFEC20455)
#define EXT_OPN2_56_ADSR__SR_AR__CH3_OP2		((uint8_t* const) 0xFEC20456)
#define EXT_OPN2_58_ADSR__SR_AR__CH1_OP3		((uint8_t* const) 0xFEC20458)
#define EXT_OPN2_59_ADSR__SR_AR__CH2_OP3		((uint8_t* const) 0xFEC20459)
#define EXT_OPN2_5A_ADSR__SR_AR__CH3_OP3		((uint8_t* const) 0xFEC2045A)
#define EXT_OPN2_5C_ADSR__SR_AR__CH1_OP4		((uint8_t* const) 0xFEC2045C)
#define EXT_OPN2_5D_ADSR__SR_AR__CH2_OP4		((uint8_t* const) 0xFEC2045D)
#define EXT_OPN2_5E_ADSR__SR_AR__CH3_OP4		((uint8_t* const) 0xFEC2045E)
#define EXT_OPN2_50_ADSR__SR_AR__CH1_OP5		((uint8_t* const) 0xFEC20550)
#define EXT_OPN2_51_ADSR__SR_AR__CH2_OP5		((uint8_t* const) 0xFEC20551)
#define EXT_OPN2_52_ADSR__SR_AR__CH3_OP5		((uint8_t* const) 0xFEC20552)
#define EXT_OPN2_54_ADSR__SR_AR__CH1_OP6		((uint8_t* const) 0xFEC20554)
#define EXT_OPN2_55_ADSR__SR_AR__CH2_OP6		((uint8_t* const) 0xFEC20555)
#define EXT_OPN2_56_ADSR__SR_AR__CH3_OP6		((uint8_t* const) 0xFEC20556)
#define EXT_OPN2_58_ADSR__SR_AR__CH1_OP7		((uint8_t* const) 0xFEC20558)
#define EXT_OPN2_59_ADSR__SR_AR__CH2_OP7		((uint8_t* const) 0xFEC20559)
#define EXT_OPN2_5A_ADSR__SR_AR__CH3_OP7		((uint8_t* const) 0xFEC2055A)
#define EXT_OPN2_5C_ADSR__SR_AR__CH1_OP8		((uint8_t* const) 0xFEC2055C)
#define EXT_OPN2_5D_ADSR__SR_AR__CH2_OP8		((uint8_t* const) 0xFEC2055D)
#define EXT_OPN2_5E_ADSR__SR_AR__CH3_OP8		((uint8_t* const) 0xFEC2055E)
//)--------------------------------------------------------------------------------------
//)-------------- AM modulation enable amd D1R (first decay rate) -----------------------
//)--------------------------------------------------------------------------------------
//)
//) D1R (first decay rate) is the initial steep amplitude decay rate (see page 4). It is,
//) like all rates, 0-31 in value and affected by RS.
//)
//) AM is the amplitude modulation enable, whether of not this operator will be subject to
//) amplitude modulation by the LFO. This bit is not relevant unless both the LFO is
//) enabled and register B4′s AMS (amplitude modulation sensitivity) is non-zero.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_60_ADSR__AM_D1R__CH1_OP1		((uint8_t* const) 0xFEC20460)
#define EXT_OPN2_61_ADSR__AM_D1R__CH2_OP1		((uint8_t* const) 0xFEC20461)
#define EXT_OPN2_62_ADSR__AM_D1R__CH3_OP1		((uint8_t* const) 0xFEC20462)
#define EXT_OPN2_64_ADSR__AM_D1R__CH1_OP2		((uint8_t* const) 0xFEC20464)
#define EXT_OPN2_65_ADSR__AM_D1R__CH2_OP2		((uint8_t* const) 0xFEC20465)
#define EXT_OPN2_66_ADSR__AM_D1R__CH3_OP2		((uint8_t* const) 0xFEC20466)
#define EXT_OPN2_68_ADSR__AM_D1R__CH1_OP3		((uint8_t* const) 0xFEC20468)
#define EXT_OPN2_69_ADSR__AM_D1R__CH2_OP3		((uint8_t* const) 0xFEC20469)
#define EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP3		((uint8_t* const) 0xFEC2046A)
#define EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP4		((uint8_t* const) 0xFEC2046C)
#define EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP4		((uint8_t* const) 0xFEC2046D)
#define EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP4		((uint8_t* const) 0xFEC2046E)
#define EXT_OPN2_60_ADSR__AM_D1R__CH1_OP5		((uint8_t* const) 0xFEC20560)
#define EXT_OPN2_61_ADSR__AM_D1R__CH2_OP5		((uint8_t* const) 0xFEC20561)
#define EXT_OPN2_62_ADSR__AM_D1R__CH3_OP5		((uint8_t* const) 0xFEC20562)
#define EXT_OPN2_64_ADSR__AM_D1R__CH1_OP6		((uint8_t* const) 0xFEC20564)
#define EXT_OPN2_65_ADSR__AM_D1R__CH2_OP6		((uint8_t* const) 0xFEC20565)
#define EXT_OPN2_66_ADSR__AM_D1R__CH3_OP6		((uint8_t* const) 0xFEC20566)
#define EXT_OPN2_68_ADSR__AM_D1R__CH1_OP7		((uint8_t* const) 0xFEC20568)
#define EXT_OPN2_69_ADSR__AM_D1R__CH2_OP7		((uint8_t* const) 0xFEC20569)
#define EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP7		((uint8_t* const) 0xFEC2056A)
#define EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP8		((uint8_t* const) 0xFEC2056C)
#define EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP8		((uint8_t* const) 0xFEC2056D)
#define EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP8		((uint8_t* const) 0xFEC2056E)
//)--------------------------------------------------------------------------------------
//)-------------------------- D2R (secondary decay rate) --------------------------------
//)--------------------------------------------------------------------------------------
//)
//) D2R (secondary decay rate) is the long tail off of the sound that continues as long
//) as the key is depressed.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_70_ADSR__D2R__CH1_OP1			((uint8_t* const) 0xFEC20470)	
#define EXT_OPN2_71_ADSR__D2R__CH2_OP1			((uint8_t* const) 0xFEC20471)	
#define EXT_OPN2_72_ADSR__D2R__CH3_OP1			((uint8_t* const) 0xFEC20472)	
#define EXT_OPN2_74_ADSR__D2R__CH1_OP2			((uint8_t* const) 0xFEC20474)	
#define EXT_OPN2_75_ADSR__D2R__CH2_OP2			((uint8_t* const) 0xFEC20475)	
#define EXT_OPN2_76_ADSR__D2R__CH3_OP2			((uint8_t* const) 0xFEC20476)	
#define EXT_OPN2_78_ADSR__D2R__CH1_OP3			((uint8_t* const) 0xFEC20478)	
#define EXT_OPN2_79_ADSR__D2R__CH2_OP3			((uint8_t* const) 0xFEC20479)	
#define EXT_OPN2_7A_ADSR__D2R__CH3_OP3			((uint8_t* const) 0xFEC2047A)	
#define EXT_OPN2_7C_ADSR__D2R__CH1_OP4			((uint8_t* const) 0xFEC2047C)	
#define EXT_OPN2_7D_ADSR__D2R__CH2_OP4			((uint8_t* const) 0xFEC2047D)	
#define EXT_OPN2_7E_ADSR__D2R__CH3_OP4			((uint8_t* const) 0xFEC2047E)	
#define EXT_OPN2_70_ADSR__D2R__CH1_OP5			((uint8_t* const) 0xFEC20570)
#define EXT_OPN2_71_ADSR__D2R__CH2_OP5			((uint8_t* const) 0xFEC20571)
#define EXT_OPN2_72_ADSR__D2R__CH3_OP5			((uint8_t* const) 0xFEC20572)
#define EXT_OPN2_74_ADSR__D2R__CH1_OP6			((uint8_t* const) 0xFEC20574)
#define EXT_OPN2_75_ADSR__D2R__CH2_OP6			((uint8_t* const) 0xFEC20575)
#define EXT_OPN2_76_ADSR__D2R__CH3_OP6			((uint8_t* const) 0xFEC20576)
#define EXT_OPN2_78_ADSR__D2R__CH1_OP7			((uint8_t* const) 0xFEC20578)
#define EXT_OPN2_79_ADSR__D2R__CH2_OP7			((uint8_t* const) 0xFEC20579)
#define EXT_OPN2_7A_ADSR__D2R__CH3_OP7			((uint8_t* const) 0xFEC2057A)
#define EXT_OPN2_7C_ADSR__D2R__CH1_OP8			((uint8_t* const) 0xFEC2057C)
#define EXT_OPN2_7D_ADSR__D2R__CH2_OP8			((uint8_t* const) 0xFEC2057D)
#define EXT_OPN2_7E_ADSR__D2R__CH3_OP8			((uint8_t* const) 0xFEC2057E)
//)--------------------------------------------------------------------------------------
//)-------------------------- D2R (secondary decay rate) --------------------------------
//)--------------------------------------------------------------------------------------
//)
//) D2R (secondary decay rate) is the long tail off of the sound that continues as long
//) as the key is depressed.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_80_ADSR__D1L_RR__CH1_OP1		((uint8_t* const) 0xFEC20480)
#define EXT_OPN2_81_ADSR__D1L_RR__CH2_OP1		((uint8_t* const) 0xFEC20481)
#define EXT_OPN2_82_ADSR__D1L_RR__CH3_OP1		((uint8_t* const) 0xFEC20482)
#define EXT_OPN2_84_ADSR__D1L_RR__CH1_OP2		((uint8_t* const) 0xFEC20484)
#define EXT_OPN2_85_ADSR__D1L_RR__CH2_OP2		((uint8_t* const) 0xFEC20485)
#define EXT_OPN2_86_ADSR__D1L_RR__CH3_OP2		((uint8_t* const) 0xFEC20486)
#define EXT_OPN2_88_ADSR__D1L_RR__CH1_OP3		((uint8_t* const) 0xFEC20488)
#define EXT_OPN2_89_ADSR__D1L_RR__CH2_OP3		((uint8_t* const) 0xFEC20489)
#define EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP3		((uint8_t* const) 0xFEC2048A)
#define EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP4		((uint8_t* const) 0xFEC2048C)
#define EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP4		((uint8_t* const) 0xFEC2048D)
#define EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP4		((uint8_t* const) 0xFEC2048E)
#define EXT_OPN2_80_ADSR__D1L_RR__CH1_OP5		((uint8_t* const) 0xFEC20580)
#define EXT_OPN2_81_ADSR__D1L_RR__CH2_OP5		((uint8_t* const) 0xFEC20581)
#define EXT_OPN2_82_ADSR__D1L_RR__CH3_OP5		((uint8_t* const) 0xFEC20582)
#define EXT_OPN2_84_ADSR__D1L_RR__CH1_OP6		((uint8_t* const) 0xFEC20584)
#define EXT_OPN2_85_ADSR__D1L_RR__CH2_OP6		((uint8_t* const) 0xFEC20585)
#define EXT_OPN2_86_ADSR__D1L_RR__CH3_OP6		((uint8_t* const) 0xFEC20586)
#define EXT_OPN2_88_ADSR__D1L_RR__CH1_OP7		((uint8_t* const) 0xFEC20588)
#define EXT_OPN2_89_ADSR__D1L_RR__CH2_OP7		((uint8_t* const) 0xFEC20589)
#define EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP7		((uint8_t* const) 0xFEC2058A)
#define EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP8		((uint8_t* const) 0xFEC2058C)
#define EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP8		((uint8_t* const) 0xFEC2058D)
#define EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP8		((uint8_t* const) 0xFEC2058E)
//)--------------------------------------------------------------------------------------
//)------------------------------------ SSG-EG ------------------------------------------
//)--------------------------------------------------------------------------------------
//)
//) This register is proprietary and should be set to zero.
//)
//)--------------------------------------------------------------------------------------
#define EXT_OPN2_90_ADSR__D1L_RR__CH1_OP1		((uint8_t* const) 0xFEC20490)
#define EXT_OPN2_91_ADSR__D1L_RR__CH2_OP1		((uint8_t* const) 0xFEC20491)
#define EXT_OPN2_92_ADSR__D1L_RR__CH3_OP1		((uint8_t* const) 0xFEC20492)
#define EXT_OPN2_94_ADSR__D1L_RR__CH1_OP2		((uint8_t* const) 0xFEC20494)
#define EXT_OPN2_95_ADSR__D1L_RR__CH2_OP2		((uint8_t* const) 0xFEC20495)
#define EXT_OPN2_96_ADSR__D1L_RR__CH3_OP2		((uint8_t* const) 0xFEC20496)
#define EXT_OPN2_98_ADSR__D1L_RR__CH1_OP3		((uint8_t* const) 0xFEC20498)
#define EXT_OPN2_99_ADSR__D1L_RR__CH2_OP3		((uint8_t* const) 0xFEC20499)
#define EXT_OPN2_9A_ADSR__D1L_RR__CH3_OP3		((uint8_t* const) 0xFEC2049A)
#define EXT_OPN2_9C_ADSR__D1L_RR__CH1_OP4		((uint8_t* const) 0xFEC2049C)
#define EXT_OPN2_9D_ADSR__D1L_RR__CH2_OP4		((uint8_t* const) 0xFEC2049D)
#define EXT_OPN2_9E_ADSR__D1L_RR__CH3_OP4		((uint8_t* const) 0xFEC2049E)
#define EXT_OPN2_90_ADSR__D1L_RR__CH4_OP1		((uint8_t* const) 0xFEC20590)
#define EXT_OPN2_91_ADSR__D1L_RR__CH5_OP1		((uint8_t* const) 0xFEC20591)
#define EXT_OPN2_92_ADSR__D1L_RR__CH6_OP1		((uint8_t* const) 0xFEC20592)
#define EXT_OPN2_94_ADSR__D1L_RR__CH4_OP2		((uint8_t* const) 0xFEC20594)
#define EXT_OPN2_95_ADSR__D1L_RR__CH5_OP2		((uint8_t* const) 0xFEC20595)
#define EXT_OPN2_96_ADSR__D1L_RR__CH6_OP2		((uint8_t* const) 0xFEC20596)
#define EXT_OPN2_98_ADSR__D1L_RR__CH4_OP3		((uint8_t* const) 0xFEC20598)
#define EXT_OPN2_99_ADSR__D1L_RR__CH5_OP3		((uint8_t* const) 0xFEC20599)
#define EXT_OPN2_9A_ADSR__D1L_RR__CH6_OP3		((uint8_t* const) 0xFEC2059A)
#define EXT_OPN2_9C_ADSR__D1L_RR__CH4_OP4		((uint8_t* const) 0xFEC2059C)
#define EXT_OPN2_9D_ADSR__D1L_RR__CH5_OP4		((uint8_t* const) 0xFEC2059D)
#define EXT_OPN2_9E_ADSR__D1L_RR__CH6_OP4		((uint8_t* const) 0xFEC2059E)
//)--------------------------------------------------------------------------------------

#define EXT_OPN2_A0_CH1_FREQ_L				((uint8_t* const) 0xFEC204A0) 	//Frequency number LSB
#define EXT_OPN2_A1_CH2_FREQ_L				((uint8_t* const) 0xFEC204A1) 	//Frequency number LSB
#define EXT_OPN2_A2_CH3_FREQ_L				((uint8_t* const) 0xFEC204A2) 	//Frequency number LSB
#define EXT_OPN2_A2_CH3_FREQ_OP1_L			((uint8_t* const) 0xFEC204A2) 	//Frequency number LSB Chanel 3 Operator 1 if special mode
#define EXT_OPN2_A8_CH3_FREQ_OP2_L			((uint8_t* const) 0xFEC204A8) 	//Frequency number LSB Chanel 3 Operator 2 if special mode
#define EXT_OPN2_A9_CH3_FREQ_OP3_L			((uint8_t* const) 0xFEC204A9) 	//Frequency number LSB Chanel 3 Operator 3 if special mode
#define EXT_OPN2_AA_CH3_FREQ_OP4_L			((uint8_t* const) 0xFEC204AA) 	//Frequency number LSB Chanel 3 Operator 4 if special mode
#define EXT_OPN2_A0_CH4_FREQ_L				((uint8_t* const) 0xFEC205A0) 	//Frequency number LSB
#define EXT_OPN2_A1_CH5_FREQ_L				((uint8_t* const) 0xFEC205A1) 	//Frequency number LSB
#define EXT_OPN2_A2_CH6_FREQ_L				((uint8_t* const) 0xFEC205A2) 	//Frequency number LSB

#define EXT_OPN2_A4_CH1_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC204A4)	//OCTAVE and Frequency number MSB
#define EXT_OPN2_A5_CH2_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC204A5)	//OCTAVE and Frequency number MSB
#define EXT_OPN2_A6_CH3_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC204A6)	//OCTAVE and Frequency number MSB
#define EXT_OPN2_A6_CH3_OCTAVE_FREQ_OP1_H	((uint8_t* const) 0xFEC204A6)	//OCTAVE and Frequency number MSB Chanel 3 Operator 1 if special mode
#define EXT_OPN2_AC_CH3_OCTAVE_FREQ_OP2_H	((uint8_t* const) 0xFEC204AC)	//OCTAVE and Frequency number MSB Chanel 3 Operator 2 if special mode
#define EXT_OPN2_AD_CH3_OCTAVE_FREQ_OP3_H	((uint8_t* const) 0xFEC204AD)	//OCTAVE and Frequency number MSB Chanel 3 Operator 3 if special mode
#define EXT_OPN2_AE_CH3_OCTAVE_FREQ_OP4_H	((uint8_t* const) 0xFEC204AE)	//OCTAVE and Frequency number MSB Chanel 3 Operator 4 if special mode
#define EXT_OPN2_A4_CH4_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC205A4)	//OCTAVE and Frequency number MSB
#define EXT_OPN2_A5_CH5_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC205A5)	//OCTAVE and Frequency number MSB
#define EXT_OPN2_A6_CH6_OCTAVE_FREQ_H		((uint8_t* const) 0xFEC205A6)	//OCTAVE and Frequency number MSB

//) Feedback is the degree to which operator 1 feeds back into itself.
//) The algorithm is the type of inter-operator connection used
#define EXT_OPN2_B0_CH1_FEEDBACK_ALGO			((uint8_t* const)0xFEC204B0)
#define EXT_OPN2_B1_CH2_FEEDBACK_ALGO			((uint8_t* const)0xFEC204B1)
#define EXT_OPN2_B2_CH3_FEEDBACK_ALGO			((uint8_t* const)0xFEC204B2)
#define EXT_OPN2_B0_CH4_FEEDBACK_ALGO			((uint8_t* const)0xFEC205B0)
#define EXT_OPN2_B1_CH5_FEEDBACK_ALGO			((uint8_t* const)0xFEC205B1)
#define EXT_OPN2_B2_CH6_FEEDBACK_ALGO			((uint8_t* const)0xFEC205B2)

//) Register B4H contains stereo output control and LFO sensitivity control.
#define EXT_OPN2_B4_CH1_L_R_AMS_FMS				((uint8_t* const)0xFEC204B0)
#define EXT_OPN2_B5_CH2_L_R_AMS_FMS				((uint8_t* const)0xFEC204B1)
#define EXT_OPN2_B6_CH3_L_R_AMS_FMS				((uint8_t* const)0xFEC204B2)
#define EXT_OPN2_B4_CH4_L_R_AMS_FMS				((uint8_t* const)0xFEC205B0)
#define EXT_OPN2_B5_CH5_L_R_AMS_FMS				((uint8_t* const)0xFEC205B1)
#define EXT_OPN2_B6_CH6_L_R_AMS_FMS				((uint8_t* const)0xFEC205B2)

#endif


