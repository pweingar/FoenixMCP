/* Functions for the SuperIO */

#include "features.h"
#include "superio.h"

#if HAS_SUPERIO

static void init_SuperIO_config_zones(void);

/*
 * Initialize the SuperIO registers
 */
 void init_superio(void) {
   init_SuperIO_config_zones(); // This Init used to be done by the FPGA.
   
	 *GP10_REG = 0x01;
    *GP11_REG = 0x01;
    *GP12_REG = 0x01;
    *GP13_REG = 0x01;
    *GP14_REG = 0x05;
    *GP15_REG = 0x05;
    *GP16_REG = 0x05;
    *GP17_REG = 0x05;

 	*GP20_REG = 0x00;
 	*GP24_REG = 0x01;
 	*GP25_REG = 0x05;
 	*GP26_REG = 0x84;

 	*GP30_REG = 0x01;
 	*GP31_REG = 0x01;
 	*GP32_REG = 0x01;
 	*GP33_REG = 0x04; // FAN1 GPIO Config
 	*GP34_REG = 0x01;
 	*GP35_REG = 0x01;
 	*GP36_REG = 0x01;
 	*GP37_REG = 0x01;

 	*GP42_REG = 0x01;
 	*GP43_REG = 0x01;

 	*GP50_REG = 0x05;
 	*GP51_REG = 0x05;
 	*GP52_REG = 0x05;
 	*GP53_REG = 0x04;
 	*GP54_REG = 0x05;
 	*GP55_REG = 0x04;
 	*GP56_REG = 0x05;
 	*GP57_REG = 0x04;

 	*GP60_REG = 0x84;
 	*GP61_REG = 0x84;

 	*GP1_REG = 0x00;
 	*GP2_REG = 0x01;
 	*GP3_REG = 0x00;
 	*GP4_REG = 0x00;
 	*GP5_REG = 0x00;
 	*GP6_REG = 0x00;

 	*LED1_REG = 0x01;
 	*LED2_REG = 0x02;

    *FAN1_REG = 0xE0;       // <= Value to change to Get the Fan running.
                            // See doc for more options, need to set $80 to get it started and use other bits to change the PWN...
    *FAN_CTRL_REG = 0x01;
 }

static void init_SuperIO_config_zones(void) { 	
    // First step is to get into the Configuration Mode
    *CONFIG_0x2E_REG = 0x55;    // We need to Get into the Config Mode with 0x55

    // Setting Up Device 0 - Floppy 
    // {8'h06,16'h03F0,8'h00};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x00;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x03;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0xF0;
    // INT 
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x06;
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;   

    // Setting Up Device 3 - Parallel Port 
    // {8'h07,16'h0378,8'h03};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x03;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x03;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0x78;
    // INT0 
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x07;
    // Parallel Mode 
    *CONFIG_0x2E_REG = 0xF0;
    *CONFIG_0x2F_REG = 0x3A;
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;    

    // Setting Up Device 4 - Serial Port 1 
    // {8'h04,16'h03F8,8'h04};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x04;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x03;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0xF8;
    // INT0 
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x04;
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;

     // Setting Up Device 5 - Serial Port 2 
    // {8'h03,16'h02F8,8'h05};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x05;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x02;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0xF8;
    // INT0 
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x03;
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;  

     // Setting Up Device 7 - Keyboard 
    // {8'h01, 16'h0060,8'h07};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x07;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x00;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0x60;
    // INT0 (Keyboard)
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x01;
    // INT1 (mouse)
    *CONFIG_0x2E_REG = 0x72;
    *CONFIG_0x2F_REG = 0x02;    
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;

      // Setting Up Device 9 - Game Port
    // {8'h00, 16'h0200,8'h09};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x09;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x02;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0x00;
    // INT0
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x00; 
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;

      // Setting Up Device 10 - PME (Power Management)
    // {8'h00, 16'h0100,8'h0A};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x0A;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x01;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0x00;
    // INT0
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x00; 
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;

    // Setting Up Device 11 - PME (Power Management)
    // {8'h05,16'h0330,8'h0B};
    // LD
    *CONFIG_0x2E_REG = 0x07;
    *CONFIG_0x2F_REG = 0x0B;
    // BA_H (Base address)
    *CONFIG_0x2E_REG = 0x60;
    *CONFIG_0x2F_REG = 0x03;
    // BA_L
    *CONFIG_0x2E_REG = 0x61;
    *CONFIG_0x2F_REG = 0x30;
    // INT0
    *CONFIG_0x2E_REG = 0x70;
    *CONFIG_0x2F_REG = 0x05; 
    // Enable the Zone
    *CONFIG_0x2E_REG = 0x30;
    *CONFIG_0x2F_REG = 0x01;

    // Supplemental Settings
     // Power On Device 
    *CONFIG_0x2E_REG = 0x22;
    *CONFIG_0x2F_REG = 0xFF;   
    // We are done with config. 
    *CONFIG_0x2E_REG = 0xAA;    // We need to Get into the Config Mode with 0x55     

    *GP60_REG = 0x84;           // THis is to replicate the FPGA behavior when it did the config.
    *LED1_REG = 0x01;           // THis is to replace the FPGA behavior when it did the config in hardware.
 }
#endif
