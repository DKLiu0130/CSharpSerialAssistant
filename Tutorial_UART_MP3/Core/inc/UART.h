#ifndef __UART_H
#define __UART_H 	   
#include "ti_msp_dl_config.h"
#include "Vector.h"
#include <stdio.h>

extern Vector* Uart_Buffer;

void UART_init();
void MCUTransData8(const UART_Regs * UART_Port, const char *data, const uint16_t length);
void MCUTransData16(const UART_Regs * UART_Port, const uint16_t *data, const uint16_t length);

#endif
