#ifndef __KEYBOARD_H
#define __KEYBOARD_H			  	
#include "ti_msp_dl_config.h"
//#include "sys.h"
//#include "stdlib.h"	  

static const char Keys[17] = {0x00,'7','4','1','x','8','5','2','0','9','6','3','.','+','-','*','/'};
																//  1		2		3		4		5		6		7		8		9		10	11	12	13	14	15	16

//#define
#define CPU_Frq 80000 	//Unit:kHz
	#define delay_level 0 	//Unit:ms
 		     


void Keyboard_init();
inline bool readButton(size_t B);

uint16_t KeySCInput();	//Key-scanning-input;
//uint16_t KeyIRInput();	//Key-Interrupt-input;Not good to use.

#endif  
	 

