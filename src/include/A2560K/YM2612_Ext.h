#ifndef YM2612_Ext_H_   
#define YM2612_Ext_H_


// External Registers $00C20400..$00C20BFF
unsigned char * EXT_OPN2_22_LFO = 			(void *)0xFEC20422; // LFO enable 	|	LFO frequency
unsigned char * EXT_OPN2_23_TIMER_A_H = 	(void *)0xFEC20423; // Timer A MSBs
unsigned char * EXT_OPN2_24_TIMER_A_L = 	(void *)0xFEC20424; // Timer A LSBs
unsigned char * EXT_OPN2_25_TIMER_B = 		(void *)0xFEC20425; // Timer B
unsigned char * EXT_OPN2_27_CHANEL_3_MODE = (void *)0xFEC20427; // Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
unsigned char * EXT_OPN2_27_TIMER_CONF = 	(void *)0xFEC20427; // Ch3 mode 	Reset B 	Reset A 	Enable B 	Enable A 	Load B 	Load A
unsigned char * EXT_OPN2_28_KEY_ON_OFF = 	(void *)0xFEC20428; // Operator Channel
unsigned char * EXT_OPN2_29	= 				(void *)0xFEC20429;
unsigned char * EXT_OPN2_2A_ADC = 			(void *)0xFEC2042A; // DAC
unsigned char * EXT_OPN2_2B_ADC_EN = 		(void *)0xFEC2042B; // DAC en
//;--------------------------------------------------------------------------------------
//;------------------- DT1 (detune) and MUL (multiple) ----------------------------------
//;--------------------------------------------------------------------------------------
//;
//; MUL ranges from 0 to 15 (decimal), and multiplies the overall frequency, with the
//; exception that 0 results in multiplication by 1/2. That is, MUL=0 to 15 gives ×1/2,
//; ×1, ×2, ... ×15.
//;
//; DT1 gives small variations from the overall frequency × MUL. The MSB of DT1 is a
//; primitive sign bit, and the two LSB’s are magnitude bits. See the next page for a
//; diagram.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP1		= (void *) 0xFEC20430;
unsigned char * EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP1		= (void *) 0xFEC20431;
unsigned char * EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP1		= (void *) 0xFEC20432;
unsigned char * EXT_OPN2_34_ADSR__DT1_MUL__CH1_OP2		= (void *) 0xFEC20434;
unsigned char * EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP2		= (void *) 0xFEC20435;
unsigned char * EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP2		= (void *) 0xFEC20436;
unsigned char * EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP3		= (void *) 0xFEC20438;
unsigned char * EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP3		= (void *) 0xFEC20439;
unsigned char * EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP3		= (void *) 0xFEC2043A;
unsigned char * EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP4		= (void *) 0xFEC2043C;
unsigned char * EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP4		= (void *) 0xFEC2043D;
unsigned char * EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP4		= (void *) 0xFEC2043E;
unsigned char * EXT_OPN2_30_ADSR__DT1_MUL__CH1_OP5		= (void *) 0xFEC20530;
unsigned char * EXT_OPN2_31_ADSR__DT1_MUL__CH2_OP5		= (void *) 0xFEC20531;
unsigned char * EXT_OPN2_32_ADSR__DT1_MUL__CH3_OP5		= (void *) 0xFEC20532;
unsigned char * EXT_OPN2_34_ADSR__DT1_MUL__CH1_OP6		= (void *) 0xFEC20534;
unsigned char * EXT_OPN2_35_ADSR__DT1_MUL__CH2_OP6		= (void *) 0xFEC20535;
unsigned char * EXT_OPN2_36_ADSR__DT1_MUL__CH3_OP6		= (void *) 0xFEC20536;
unsigned char * EXT_OPN2_38_ADSR__DT1_MUL__CH1_OP7		= (void *) 0xFEC20538;
unsigned char * EXT_OPN2_39_ADSR__DT1_MUL__CH2_OP7		= (void *) 0xFEC20539;
unsigned char * EXT_OPN2_3A_ADSR__DT1_MUL__CH3_OP7		= (void *) 0xFEC2053A;
unsigned char * EXT_OPN2_3C_ADSR__DT1_MUL__CH1_OP8		= (void *) 0xFEC2053C;
unsigned char * EXT_OPN2_3D_ADSR__DT1_MUL__CH2_OP8		= (void *) 0xFEC2053D;
unsigned char * EXT_OPN2_3E_ADSR__DT1_MUL__CH3_OP8		= (void *) 0xFEC2053E;

//;--------------------------------------------------------------------------------------
//;---------------------------------- TL (total level) ----------------------------------
//;--------------------------------------------------------------------------------------
//;
//; TL (total level) represents the envelope’s highest amplitude, with 0 being the largest
//; and 127 (decimal) the smallest. A change of one unit is about 0.75 dB.
//;
//; To make a note softer, only change the TL of the slots (the output operators).
//; Changing the other operators will affect the flavor of the note.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_40_ADSR__LT__CH1_OP1		= (void *) 0xFEC20440;
unsigned char * EXT_OPN2_41_ADSR__LT__CH2_OP1		= (void *) 0xFEC20441;
unsigned char * EXT_OPN2_42_ADSR__LT__CH3_OP1		= (void *) 0xFEC20442;
unsigned char * EXT_OPN2_44_ADSR__LT__CH1_OP2		= (void *) 0xFEC20444;
unsigned char * EXT_OPN2_45_ADSR__LT__CH2_OP2		= (void *) 0xFEC20445;
unsigned char * EXT_OPN2_46_ADSR__LT__CH3_OP2		= (void *) 0xFEC20446;
unsigned char * EXT_OPN2_48_ADSR__LT__CH1_OP3		= (void *) 0xFEC20448;
unsigned char * EXT_OPN2_49_ADSR__LT__CH2_OP3		= (void *) 0xFEC20449;
unsigned char * EXT_OPN2_4A_ADSR__LT__CH3_OP3		= (void *) 0xFEC2044A;
unsigned char * EXT_OPN2_4C_ADSR__LT__CH1_OP4		= (void *) 0xFEC2044C;
unsigned char * EXT_OPN2_4D_ADSR__LT__CH2_OP4		= (void *) 0xFEC2044D;
unsigned char * EXT_OPN2_4E_ADSR__LT__CH3_OP4		= (void *) 0xFEC2044E;
unsigned char * EXT_OPN2_40_ADSR__LT__CH1_OP5		= (void *) 0xFEC20540;
unsigned char * EXT_OPN2_41_ADSR__LT__CH2_OP5		= (void *) 0xFEC20541;
unsigned char * EXT_OPN2_42_ADSR__LT__CH3_OP5		= (void *) 0xFEC20542;
unsigned char * EXT_OPN2_44_ADSR__LT__CH1_OP6		= (void *) 0xFEC20544;
unsigned char * EXT_OPN2_45_ADSR__LT__CH2_OP6		= (void *) 0xFEC20545;
unsigned char * EXT_OPN2_46_ADSR__LT__CH3_OP6		= (void *) 0xFEC20546;
unsigned char * EXT_OPN2_48_ADSR__LT__CH1_OP7		= (void *) 0xFEC20548;
unsigned char * EXT_OPN2_49_ADSR__LT__CH2_OP7		= (void *) 0xFEC20549;
unsigned char * EXT_OPN2_4A_ADSR__LT__CH3_OP7		= (void *) 0xFEC2054A;
unsigned char * EXT_OPN2_4C_ADSR__LT__CH1_OP8		= (void *) 0xFEC2054C;
unsigned char * EXT_OPN2_4D_ADSR__LT__CH2_OP8		= (void *) 0xFEC2054D;
unsigned char * EXT_OPN2_4E_ADSR__LT__CH3_OP8		= (void *) 0xFEC2054E;
//;--------------------------------------------------------------------------------------
//;------------------- RS (rate scaling) and AR (attack rate) ---------------------------
//;--------------------------------------------------------------------------------------
//;
//; AR is the steepness of the initial amplitude rise, shown on page 5.
//;
//; RS affects AR, D1R, D2R and RR in the same way. RS is the degree to which the envelope
//; becomes narrower as the frequency becomes higher.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_50_ADSR__SR_AR__CH1_OP1		= (void *) 0xFEC20450;
unsigned char * EXT_OPN2_51_ADSR__SR_AR__CH2_OP1		= (void *) 0xFEC20451;
unsigned char * EXT_OPN2_52_ADSR__SR_AR__CH3_OP1		= (void *) 0xFEC20452;
unsigned char * EXT_OPN2_54_ADSR__SR_AR__CH1_OP2		= (void *) 0xFEC20454;
unsigned char * EXT_OPN2_55_ADSR__SR_AR__CH2_OP2		= (void *) 0xFEC20455;
unsigned char * EXT_OPN2_56_ADSR__SR_AR__CH3_OP2		= (void *) 0xFEC20456;
unsigned char * EXT_OPN2_58_ADSR__SR_AR__CH1_OP3		= (void *) 0xFEC20458;
unsigned char * EXT_OPN2_59_ADSR__SR_AR__CH2_OP3		= (void *) 0xFEC20459;
unsigned char * EXT_OPN2_5A_ADSR__SR_AR__CH3_OP3		= (void *) 0xFEC2045A;
unsigned char * EXT_OPN2_5C_ADSR__SR_AR__CH1_OP4		= (void *) 0xFEC2045C;
unsigned char * EXT_OPN2_5D_ADSR__SR_AR__CH2_OP4		= (void *) 0xFEC2045D;
unsigned char * EXT_OPN2_5E_ADSR__SR_AR__CH3_OP4		= (void *) 0xFEC2045E;
unsigned char * EXT_OPN2_50_ADSR__SR_AR__CH1_OP5		= (void *) 0xFEC20550;
unsigned char * EXT_OPN2_51_ADSR__SR_AR__CH2_OP5		= (void *) 0xFEC20551;
unsigned char * EXT_OPN2_52_ADSR__SR_AR__CH3_OP5		= (void *) 0xFEC20552;
unsigned char * EXT_OPN2_54_ADSR__SR_AR__CH1_OP6		= (void *) 0xFEC20554;
unsigned char * EXT_OPN2_55_ADSR__SR_AR__CH2_OP6		= (void *) 0xFEC20555;
unsigned char * EXT_OPN2_56_ADSR__SR_AR__CH3_OP6		= (void *) 0xFEC20556;
unsigned char * EXT_OPN2_58_ADSR__SR_AR__CH1_OP7		= (void *) 0xFEC20558;
unsigned char * EXT_OPN2_59_ADSR__SR_AR__CH2_OP7		= (void *) 0xFEC20559;
unsigned char * EXT_OPN2_5A_ADSR__SR_AR__CH3_OP7		= (void *) 0xFEC2055A;
unsigned char * EXT_OPN2_5C_ADSR__SR_AR__CH1_OP8		= (void *) 0xFEC2055C;
unsigned char * EXT_OPN2_5D_ADSR__SR_AR__CH2_OP8		= (void *) 0xFEC2055D;
unsigned char * EXT_OPN2_5E_ADSR__SR_AR__CH3_OP8		= (void *) 0xFEC2055E;
//;--------------------------------------------------------------------------------------
//;-------------- AM modulation enable amd D1R (first decay rate) -----------------------
//;--------------------------------------------------------------------------------------
//;
//; D1R (first decay rate) is the initial steep amplitude decay rate (see page 4). It is,
//; like all rates, 0-31 in value and affected by RS.
//;
//; AM is the amplitude modulation enable, whether of not this operator will be subject to
//; amplitude modulation by the LFO. This bit is not relevant unless both the LFO is
//; enabled and register B4′s AMS (amplitude modulation sensitivity) is non-zero.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_60_ADSR__AM_D1R__CH1_OP1		= (void *) 0xFEC20460;
unsigned char * EXT_OPN2_61_ADSR__AM_D1R__CH2_OP1		= (void *) 0xFEC20461;
unsigned char * EXT_OPN2_62_ADSR__AM_D1R__CH3_OP1		= (void *) 0xFEC20462;
unsigned char * EXT_OPN2_64_ADSR__AM_D1R__CH1_OP2		= (void *) 0xFEC20464;
unsigned char * EXT_OPN2_65_ADSR__AM_D1R__CH2_OP2		= (void *) 0xFEC20465;
unsigned char * EXT_OPN2_66_ADSR__AM_D1R__CH3_OP2		= (void *) 0xFEC20466;
unsigned char * EXT_OPN2_68_ADSR__AM_D1R__CH1_OP3		= (void *) 0xFEC20468;
unsigned char * EXT_OPN2_69_ADSR__AM_D1R__CH2_OP3		= (void *) 0xFEC20469;
unsigned char * EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP3		= (void *) 0xFEC2046A;
unsigned char * EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP4		= (void *) 0xFEC2046C;
unsigned char * EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP4		= (void *) 0xFEC2046D;
unsigned char * EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP4		= (void *) 0xFEC2046E;
unsigned char * EXT_OPN2_60_ADSR__AM_D1R__CH1_OP5		= (void *) 0xFEC20560;
unsigned char * EXT_OPN2_61_ADSR__AM_D1R__CH2_OP5		= (void *) 0xFEC20561;
unsigned char * EXT_OPN2_62_ADSR__AM_D1R__CH3_OP5		= (void *) 0xFEC20562;
unsigned char * EXT_OPN2_64_ADSR__AM_D1R__CH1_OP6		= (void *) 0xFEC20564;
unsigned char * EXT_OPN2_65_ADSR__AM_D1R__CH2_OP6		= (void *) 0xFEC20565;
unsigned char * EXT_OPN2_66_ADSR__AM_D1R__CH3_OP6		= (void *) 0xFEC20566;
unsigned char * EXT_OPN2_68_ADSR__AM_D1R__CH1_OP7		= (void *) 0xFEC20568;
unsigned char * EXT_OPN2_69_ADSR__AM_D1R__CH2_OP7		= (void *) 0xFEC20569;
unsigned char * EXT_OPN2_6A_ADSR__AM_D1R__CH3_OP7		= (void *) 0xFEC2056A;
unsigned char * EXT_OPN2_6C_ADSR__AM_D1R__CH1_OP8		= (void *) 0xFEC2056C;
unsigned char * EXT_OPN2_6D_ADSR__AM_D1R__CH2_OP8		= (void *) 0xFEC2056D;
unsigned char * EXT_OPN2_6E_ADSR__AM_D1R__CH3_OP8		= (void *) 0xFEC2056E;
//;--------------------------------------------------------------------------------------
//;-------------------------- D2R (secondary decay rate) --------------------------------
//;--------------------------------------------------------------------------------------
//;
//; D2R (secondary decay rate) is the long tail off of the sound that continues as long
//; as the key is depressed.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_70_ADSR__D2R__CH1_OP1			= (void *) 0xFEC20470;	
unsigned char * EXT_OPN2_71_ADSR__D2R__CH2_OP1			= (void *) 0xFEC20471;	
unsigned char * EXT_OPN2_72_ADSR__D2R__CH3_OP1			= (void *) 0xFEC20472;	
unsigned char * EXT_OPN2_74_ADSR__D2R__CH1_OP2			= (void *) 0xFEC20474;	
unsigned char * EXT_OPN2_75_ADSR__D2R__CH2_OP2			= (void *) 0xFEC20475;	
unsigned char * EXT_OPN2_76_ADSR__D2R__CH3_OP2			= (void *) 0xFEC20476;	
unsigned char * EXT_OPN2_78_ADSR__D2R__CH1_OP3			= (void *) 0xFEC20478;	
unsigned char * EXT_OPN2_79_ADSR__D2R__CH2_OP3			= (void *) 0xFEC20479;	
unsigned char * EXT_OPN2_7A_ADSR__D2R__CH3_OP3			= (void *) 0xFEC2047A;	
unsigned char * EXT_OPN2_7C_ADSR__D2R__CH1_OP4			= (void *) 0xFEC2047C;	
unsigned char * EXT_OPN2_7D_ADSR__D2R__CH2_OP4			= (void *) 0xFEC2047D;	
unsigned char * EXT_OPN2_7E_ADSR__D2R__CH3_OP4			= (void *) 0xFEC2047E;	
unsigned char * EXT_OPN2_70_ADSR__D2R__CH1_OP5			= (void *) 0xFEC20570;
unsigned char * EXT_OPN2_71_ADSR__D2R__CH2_OP5			= (void *) 0xFEC20571;
unsigned char * EXT_OPN2_72_ADSR__D2R__CH3_OP5			= (void *) 0xFEC20572;
unsigned char * EXT_OPN2_74_ADSR__D2R__CH1_OP6			= (void *) 0xFEC20574;
unsigned char * EXT_OPN2_75_ADSR__D2R__CH2_OP6			= (void *) 0xFEC20575;
unsigned char * EXT_OPN2_76_ADSR__D2R__CH3_OP6			= (void *) 0xFEC20576;
unsigned char * EXT_OPN2_78_ADSR__D2R__CH1_OP7			= (void *) 0xFEC20578;
unsigned char * EXT_OPN2_79_ADSR__D2R__CH2_OP7			= (void *) 0xFEC20579;
unsigned char * EXT_OPN2_7A_ADSR__D2R__CH3_OP7			= (void *) 0xFEC2057A;
unsigned char * EXT_OPN2_7C_ADSR__D2R__CH1_OP8			= (void *) 0xFEC2057C;
unsigned char * EXT_OPN2_7D_ADSR__D2R__CH2_OP8			= (void *) 0xFEC2057D;
unsigned char * EXT_OPN2_7E_ADSR__D2R__CH3_OP8			= (void *) 0xFEC2057E;
//;--------------------------------------------------------------------------------------
//;-------------------------- D2R (secondary decay rate) --------------------------------
//;--------------------------------------------------------------------------------------
//;
//; D2R (secondary decay rate) is the long tail off of the sound that continues as long
//; as the key is depressed.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_80_ADSR__D1L_RR__CH1_OP1		= (void *) 0xFEC20480;
unsigned char * EXT_OPN2_81_ADSR__D1L_RR__CH2_OP1		= (void *) 0xFEC20481;
unsigned char * EXT_OPN2_82_ADSR__D1L_RR__CH3_OP1		= (void *) 0xFEC20482;
unsigned char * EXT_OPN2_84_ADSR__D1L_RR__CH1_OP2		= (void *) 0xFEC20484;
unsigned char * EXT_OPN2_85_ADSR__D1L_RR__CH2_OP2		= (void *) 0xFEC20485;
unsigned char * EXT_OPN2_86_ADSR__D1L_RR__CH3_OP2		= (void *) 0xFEC20486;
unsigned char * EXT_OPN2_88_ADSR__D1L_RR__CH1_OP3		= (void *) 0xFEC20488;
unsigned char * EXT_OPN2_89_ADSR__D1L_RR__CH2_OP3		= (void *) 0xFEC20489;
unsigned char * EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP3		= (void *) 0xFEC2048A;
unsigned char * EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP4		= (void *) 0xFEC2048C;
unsigned char * EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP4		= (void *) 0xFEC2048D;
unsigned char * EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP4		= (void *) 0xFEC2048E;
unsigned char * EXT_OPN2_80_ADSR__D1L_RR__CH1_OP5		= (void *) 0xFEC20580;
unsigned char * EXT_OPN2_81_ADSR__D1L_RR__CH2_OP5		= (void *) 0xFEC20581;
unsigned char * EXT_OPN2_82_ADSR__D1L_RR__CH3_OP5		= (void *) 0xFEC20582;
unsigned char * EXT_OPN2_84_ADSR__D1L_RR__CH1_OP6		= (void *) 0xFEC20584;
unsigned char * EXT_OPN2_85_ADSR__D1L_RR__CH2_OP6		= (void *) 0xFEC20585;
unsigned char * EXT_OPN2_86_ADSR__D1L_RR__CH3_OP6		= (void *) 0xFEC20586;
unsigned char * EXT_OPN2_88_ADSR__D1L_RR__CH1_OP7		= (void *) 0xFEC20588;
unsigned char * EXT_OPN2_89_ADSR__D1L_RR__CH2_OP7		= (void *) 0xFEC20589;
unsigned char * EXT_OPN2_8A_ADSR__D1L_RR__CH3_OP7		= (void *) 0xFEC2058A;
unsigned char * EXT_OPN2_8C_ADSR__D1L_RR__CH1_OP8		= (void *) 0xFEC2058C;
unsigned char * EXT_OPN2_8D_ADSR__D1L_RR__CH2_OP8		= (void *) 0xFEC2058D;
unsigned char * EXT_OPN2_8E_ADSR__D1L_RR__CH3_OP8		= (void *) 0xFEC2058E;
//;--------------------------------------------------------------------------------------
//;------------------------------------ SSG-EG ------------------------------------------
//;--------------------------------------------------------------------------------------
//;
//; This register is proprietary and should be set to zero.
//;
//;--------------------------------------------------------------------------------------
unsigned char * EXT_OPN2_90_ADSR__D1L_RR__CH1_OP1		= (void *) 0xFEC20490;
unsigned char * EXT_OPN2_91_ADSR__D1L_RR__CH2_OP1		= (void *) 0xFEC20491;
unsigned char * EXT_OPN2_92_ADSR__D1L_RR__CH3_OP1		= (void *) 0xFEC20492;
unsigned char * EXT_OPN2_94_ADSR__D1L_RR__CH1_OP2		= (void *) 0xFEC20494;
unsigned char * EXT_OPN2_95_ADSR__D1L_RR__CH2_OP2		= (void *) 0xFEC20495;
unsigned char * EXT_OPN2_96_ADSR__D1L_RR__CH3_OP2		= (void *) 0xFEC20496;
unsigned char * EXT_OPN2_98_ADSR__D1L_RR__CH1_OP3		= (void *) 0xFEC20498;
unsigned char * EXT_OPN2_99_ADSR__D1L_RR__CH2_OP3		= (void *) 0xFEC20499;
unsigned char * EXT_OPN2_9A_ADSR__D1L_RR__CH3_OP3		= (void *) 0xFEC2049A;
unsigned char * EXT_OPN2_9C_ADSR__D1L_RR__CH1_OP4		= (void *) 0xFEC2049C;
unsigned char * EXT_OPN2_9D_ADSR__D1L_RR__CH2_OP4		= (void *) 0xFEC2049D;
unsigned char * EXT_OPN2_9E_ADSR__D1L_RR__CH3_OP4		= (void *) 0xFEC2049E;
unsigned char * EXT_OPN2_90_ADSR__D1L_RR__CH4_OP1		= (void *) 0xFEC20590;
unsigned char * EXT_OPN2_91_ADSR__D1L_RR__CH5_OP1		= (void *) 0xFEC20591;
unsigned char * EXT_OPN2_92_ADSR__D1L_RR__CH6_OP1		= (void *) 0xFEC20592;
unsigned char * EXT_OPN2_94_ADSR__D1L_RR__CH4_OP2		= (void *) 0xFEC20594;
unsigned char * EXT_OPN2_95_ADSR__D1L_RR__CH5_OP2		= (void *) 0xFEC20595;
unsigned char * EXT_OPN2_96_ADSR__D1L_RR__CH6_OP2		= (void *) 0xFEC20596;
unsigned char * EXT_OPN2_98_ADSR__D1L_RR__CH4_OP3		= (void *) 0xFEC20598;
unsigned char * EXT_OPN2_99_ADSR__D1L_RR__CH5_OP3		= (void *) 0xFEC20599;
unsigned char * EXT_OPN2_9A_ADSR__D1L_RR__CH6_OP3		= (void *) 0xFEC2059A;
unsigned char * EXT_OPN2_9C_ADSR__D1L_RR__CH4_OP4		= (void *) 0xFEC2059C;
unsigned char * EXT_OPN2_9D_ADSR__D1L_RR__CH5_OP4		= (void *) 0xFEC2059D;
unsigned char * EXT_OPN2_9E_ADSR__D1L_RR__CH6_OP4		= (void *) 0xFEC2059E;
//;--------------------------------------------------------------------------------------

unsigned char * EXT_OPN2_A0_CH1_FREQ_L				= (void *) 0xFEC204A0; 	//Frequency number LSB
unsigned char * EXT_OPN2_A1_CH2_FREQ_L				= (void *) 0xFEC204A1; 	//Frequency number LSB
unsigned char * EXT_OPN2_A2_CH3_FREQ_L				= (void *) 0xFEC204A2; 	//Frequency number LSB
unsigned char * EXT_OPN2_A2_CH3_FREQ_OP1_L			= (void *) 0xFEC204A2; 	//Frequency number LSB Chanel 3 Operator 1 if special mode
unsigned char * EXT_OPN2_A8_CH3_FREQ_OP2_L			= (void *) 0xFEC204A8; 	//Frequency number LSB Chanel 3 Operator 2 if special mode
unsigned char * EXT_OPN2_A9_CH3_FREQ_OP3_L			= (void *) 0xFEC204A9; 	//Frequency number LSB Chanel 3 Operator 3 if special mode
unsigned char * EXT_OPN2_AA_CH3_FREQ_OP4_L			= (void *) 0xFEC204AA; 	//Frequency number LSB Chanel 3 Operator 4 if special mode
unsigned char * EXT_OPN2_A0_CH4_FREQ_L				= (void *) 0xFEC205A0; 	//Frequency number LSB
unsigned char * EXT_OPN2_A1_CH5_FREQ_L				= (void *) 0xFEC205A1; 	//Frequency number LSB
unsigned char * EXT_OPN2_A2_CH6_FREQ_L				= (void *) 0xFEC205A2; 	//Frequency number LSB

unsigned char * EXT_OPN2_A4_CH1_OCTAVE_FREQ_H		= (void *) 0xFEC204A4;	//OCTAVE and Frequency number MSB
unsigned char * EXT_OPN2_A5_CH2_OCTAVE_FREQ_H		= (void *) 0xFEC204A5;	//OCTAVE and Frequency number MSB
unsigned char * EXT_OPN2_A6_CH3_OCTAVE_FREQ_H		= (void *) 0xFEC204A6;	//OCTAVE and Frequency number MSB
unsigned char * EXT_OPN2_A6_CH3_OCTAVE_FREQ_OP1_H	= (void *) 0xFEC204A6;	//OCTAVE and Frequency number MSB Chanel 3 Operator 1 if special mode
unsigned char * EXT_OPN2_AC_CH3_OCTAVE_FREQ_OP2_H	= (void *) 0xFEC204AC;	//OCTAVE and Frequency number MSB Chanel 3 Operator 2 if special mode
unsigned char * EXT_OPN2_AD_CH3_OCTAVE_FREQ_OP3_H	= (void *) 0xFEC204AD;	//OCTAVE and Frequency number MSB Chanel 3 Operator 3 if special mode
unsigned char * EXT_OPN2_AE_CH3_OCTAVE_FREQ_OP4_H	= (void *) 0xFEC204AE;	//OCTAVE and Frequency number MSB Chanel 3 Operator 4 if special mode
unsigned char * EXT_OPN2_A4_CH4_OCTAVE_FREQ_H		= (void *) 0xFEC205A4;	//OCTAVE and Frequency number MSB
unsigned char * EXT_OPN2_A5_CH5_OCTAVE_FREQ_H		= (void *) 0xFEC205A5;	//OCTAVE and Frequency number MSB
unsigned char * EXT_OPN2_A6_CH6_OCTAVE_FREQ_H		= (void *) 0xFEC205A6;	//OCTAVE and Frequency number MSB

//; Feedback is the degree to which operator 1 feeds back into itself.
//; The algorithm is the type of inter-operator connection used
unsigned char * EXT_OPN2_B0_CH1_FEEDBACK_ALGO			= (void *)0xFEC204B0;
unsigned char * EXT_OPN2_B1_CH2_FEEDBACK_ALGO			= (void *)0xFEC204B1;
unsigned char * EXT_OPN2_B2_CH3_FEEDBACK_ALGO			= (void *)0xFEC204B2;
unsigned char * EXT_OPN2_B0_CH4_FEEDBACK_ALGO			= (void *)0xFEC205B0;
unsigned char * EXT_OPN2_B1_CH5_FEEDBACK_ALGO			= (void *)0xFEC205B1;
unsigned char * EXT_OPN2_B2_CH6_FEEDBACK_ALGO			= (void *)0xFEC205B2;

//; Register B4H contains stereo output control and LFO sensitivity control.
unsigned char * EXT_OPN2_B4_CH1_L_R_AMS_FMS				= (void *)0xFEC204B0;
unsigned char * EXT_OPN2_B5_CH2_L_R_AMS_FMS				= (void *)0xFEC204B1;
unsigned char * EXT_OPN2_B6_CH3_L_R_AMS_FMS				= (void *)0xFEC204B2;
unsigned char * EXT_OPN2_B4_CH4_L_R_AMS_FMS				= (void *)0xFEC205B0;
unsigned char * EXT_OPN2_B5_CH5_L_R_AMS_FMS				= (void *)0xFEC205B1;
unsigned char * EXT_OPN2_B6_CH6_L_R_AMS_FMS				= (void *)0xFEC205B2;

#endif


