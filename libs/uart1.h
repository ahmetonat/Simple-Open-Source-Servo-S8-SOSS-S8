
#ifndef _UART1_H_
#define _UART1_H_

#include "stm8s.h"
#define BAUD_SETT (uint32_t)115200


void UART1_Config(void);

void putchar (char c);
char getchar (void);
char getchar_nb (void);
void putstr (char* c);

void SendBufHex(char *buf, uint8_t bufsize);

#endif
