#ifndef __COMMANDLINE_H
#define __COMMANDLINE_H			  	
#include "ti_msp_dl_config.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "sys.h"
//#include "stdlib.h"	  

inline void UpperAlphabet(char *str);
inline char* DoubleSize(char * content);
void CommandLineON();
void CommandLineOFF();
uint16_t AnalyseCmd(char* CommandLine);
uint16_t embedding(char * data);


#endif  
	 


