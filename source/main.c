#include "stm32f0xx.h"                  // Device header
#include "ARM_delay.h"
#include "retarget_STM32F0.h"
#include "serial_stdio.h"
#include "GLCD.h"

void setToMaxSpeed(void);

Serial_t UART2_serial = {UART2_getChar, UART2_sendChar};
Serial_t GLCD_serial = { NULL,  glcd_putc};

int main(void)
{
	setToMaxSpeed();
	UART2_init(9600);
	serial_printf(UART2_serial,"\nSystem ready\n");
	serial_printf(UART2_serial,"$ ");
	glcd_init(CHARACTER);
	serial_printf(GLCD_serial,"\fHola mundo");
	while(1){
	}
}

void setToMaxSpeed(void){
		int internalClockCounter;
		RCC_PLLCmd(DISABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
		RCC_HSEConfig(RCC_HSE_OFF);
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);
		RCC_PREDIV1Config(RCC_PREDIV1_Div1);
		RCC_PLLCmd(ENABLE);
		while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		for(internalClockCounter=0;internalClockCounter<1024;internalClockCounter++){
			if(RCC_GetSYSCLKSource()==RCC_SYSCLKSource_PLLCLK){
				SystemCoreClockUpdate();
				break;
			}
		}		
}
