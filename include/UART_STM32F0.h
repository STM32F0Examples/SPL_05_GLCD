#ifndef __UART_STM32F0_H__
	#define	__UART_STM32F0_H__
	#include "retarget.h"
	void configureUART2(void);
	void sendchar(char c);
	char getkey(void);
#endif

