#include "stm32f0xx.h"                  // Device header
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h" 
#include "stdio.h"
#include "GLCD.h"
#include "ARM_delay.h"
#include "UART_STM32F0.h"
#include "math.h"

void setToMaxSpeed(void);
void pruebaGraficos(void);
void pruebaChar(void);
FILE GLCD_stream={(void*) glcd_putc};


void drawPushButton(char *a,unsigned char x, unsigned char y, unsigned char color){
    char c;
    int i=0;
    while((c=a[i])!='\0')i++;
    glcd_rect(x-2,y-2,x+(i*6),y+8,YES,OFF);
    if(!color) glcd_text57(x,y,a,1,ON);
    if(color) glcd_text57(x+1,y+1,a,1,ON);
    if(!color) glcd_rect(x-2,y-2,x+(i*6),y+8,NO,ON);
    if(color) glcd_rect(x-1,y-1,x+(i*6)+1,y+9,NO,ON);
}

void drawClock(unsigned char x, unsigned char y,unsigned char rad,unsigned short angle){
		 glcd_circle(80,40,10,NO,ON);
}

void drawArc(short x, short y, short r, short theta1, short theta2){
	glcd_circle(x,y,r,NO,ON);
	glcd_line(x,80,100,70,ON);
}
	

void drawSlider(){
	static int posSlide;
	static int increment;
	glcd_line(145,30,145,70,ON);
	glcd_bar(140,35+posSlide,150,35+posSlide,3,ON);
	
	if(increment==0||posSlide<=0){
		increment=2;
	}else if(posSlide>=30){
		increment=-2;
	}
	posSlide+=increment;
}

char cbuffer[10];
void pruebaBoton(){
	int i;
	int completion;
	float angle;
	completion=0;
	i=0;
	angle=0;
	glcd_init(GRAPHIC);
	glcd_fill_screen(OFF);
	while(1){
		printf("Hola mundo %d\n",i);
		glcd_fill_screen(OFF);
		snprintf(cbuffer,10,"%3d%%",completion);
		glcd_rect(10,10,110,20,NO,ON);
		glcd_rect(10,10,10+completion,20,YES,ON);
		glcd_text57(115,12,cbuffer,1,ON);
		glcd_circle(80,40,14,NO,ON);
		glcd_line(80,40,(unsigned char)(80.0+(10.0*cos(angle))),(unsigned char)(40.0+(10.0*sin(angle))),ON);
		
		drawSlider();
		drawArc(80,90,32,300,60);
		if(i){
			drawPushButton("<Hola>",12,25,ON); 
			drawPushButton("<Hola>",12,25,OFF);
			drawPushButton("Mundo!",12,40,ON);
			glcd_rect(10,55,40,65,NO,ON);
			glcd_rect(10,55,25,65,YES,ON);
		}else{
			drawPushButton(" Hola ",12,25,ON);
			drawPushButton("Mundo!",12,40,ON);
			drawPushButton("Mundo!",12,40,OFF);
			glcd_rect(10,55,40,65,NO,ON);
			glcd_rect(25,55,40,65,YES,ON);
		}
		#ifdef USE_BUFFER
		glcd_load_buffer();
		#endif
		i++;
		i&=1;
		delay_ms(250);
		angle+=3.1416/20;
		if(angle>(3.1416*2.0)){angle=0;} 
		
		completion+=2;
		if(completion>100){ 
			completion =0;
			//glcd_fill_screen(OFF);
		}
	}
}


int main(void)
{
  /* Add your application code here
     */	
	setToMaxSpeed();
	configureUART2();
  //pruebaChar();
	//pruebaGraficos();
	pruebaBoton();
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


void pruebaGraficos(void){
	int n,i;
	unsigned int ms;
	ms=250;
	n=0;
	//pruebaChar();
	glcd_init(GRAPHIC);
  /* Infinite loop */
	while (1){	
		delay_ms(ms);//genrates a ms milliseconds delay
		if((n&0x1)){
				//glcd_fill_screen(ON);
			    for(i=1;i<=100;i+=2)
					{
						glcd_circle(80,40,i,YES,OFF);
						#ifdef USE_BUFFER
								glcd_load_buffer();
						#endif
						printf("OFF=%d\n",i);
					}
		}else{
				//glcd_fill_screen(OFF);
				  for(i=1;i<=100;i+=2)
					{
						glcd_circle(80,40,i,YES,ON);
						#ifdef USE_BUFFER
								glcd_load_buffer();
						#endif
						printf("ON=%d\n",i);
					}
		}
	 					n++;
  }	
}

void pruebaChar(void){
	glcd_init(CHARACTER);
	while(1){
		fprintf(&GLCD_stream,"\fHola mundo\n");
		printf("Hola mundo\n");
		delay_ms(2000);
		fprintf(&GLCD_stream,"Esta es una GLCD en ARM\n");
		printf("Esta es una GLCD en ARM\n");
		delay_ms(2000);
		fprintf(&GLCD_stream,"\fSoy la riata\n");
		printf("Soy la riata\n");
		delay_ms(1000);
		fprintf(&GLCD_stream,"=)\n");
		printf("=)\n");
		delay_ms(3000);
	}
}


