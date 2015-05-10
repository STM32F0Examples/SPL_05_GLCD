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
	configure_delay();
	configure_delay();
	serial_printf(UART2_serial,"\nSystem ready\n");
	serial_printf(UART2_serial,"$ ");
	while(1){
		glcd_init(CHARACTER);
		serial_printf(GLCD_serial,"\fGLCD ready!");
		delay_ms(1000);
		for(int i=0 ; i<10; i++){
			serial_printf(GLCD_serial,"\fu=%d",i);
			serial_printf(GLCD_serial,"\nd=%d",i*10);
			serial_printf(GLCD_serial,"\nc=%d",i*100);
			delay_ms(250);
		}
		glcd_init(GRAPHIC);
		glcd_fill_screen(ON);
		for(int i = 0; i < 120; i+=3){
			glcd_circle(80,40,i,YES,OFF);
			glcd_load_buffer();
		}
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
