#include "stm32f0xx.h"                  // Device header
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h" 
#include "stdio.h"
#include "GLCD.h"
#include "ARM_delay.h"
#include "UART_STM32F0.h"
#include "math.h"
#include "GUI.h"
void setToMaxSpeed(void);

void pruebaChar(void);
FILE GLCD_stream={(void*) glcd_putc};


void drawPushButton(char *a,unsigned char x, unsigned char y, unsigned char color,unsigned char state);
void drawPushButton2(char *a,unsigned char x, unsigned char y, unsigned char color,unsigned char state);


/*ADC DMA STUFF*/
void configureADC(void);
void startConversion(void);
int ADC_results[4];
int trasferComplete=0;

void boton1Callback(int event_code,void* item_data);	


//char text1[]= "Hola Mundo";
GUI_Button_Def(boton1,&boton1Callback,10,10,"Hola");

void floatPin(unsigned short GPIO_PINS){
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Mode=GPIO_Mode_IN;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_Speed=GPIO_Speed_50MHz;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Pin=GPIO_PINS;
	GPIO_Init(GPIOA,&myGPIO);
}

void drivePin(unsigned short GPIO_PINS){
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);	
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_Speed=GPIO_Speed_50MHz;
	myGPIO.GPIO_PuPd=GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Pin=GPIO_PINS;
	GPIO_Init(GPIOA,&myGPIO);
}
void configureSPLYPins(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);	
	floatPin(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}

void pullDownPin(unsigned short GPIO_PINS){
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);	
	myGPIO.GPIO_Mode=GPIO_Mode_IN;
	myGPIO.GPIO_OType=GPIO_OType_PP;
	myGPIO.GPIO_Speed=GPIO_Speed_50MHz;
	myGPIO.GPIO_PuPd=GPIO_PuPd_DOWN;
	myGPIO.GPIO_Pin=GPIO_PINS;
	GPIO_Init(GPIOA,&myGPIO);
}

int detect_touch(void){
	floatPin(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	pullDownPin(GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	drivePin(GPIO_Pin_6);
	delay_us(500);
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)){
		floatPin(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		return 1;
	}else{
		floatPin(GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		return 0;
	}
}

int rawX,rawY;

void readXY(void){	
	//Prepare Pins for reading X axis
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	floatPin(GPIO_Pin_4|GPIO_Pin_5);
	drivePin(GPIO_Pin_6|GPIO_Pin_7);
	delay_us(250);
	startConversion();
	while(!trasferComplete);
	rawX=ADC_results[0];

	//Prepare Pins for reading X axis
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	floatPin(GPIO_Pin_6|GPIO_Pin_7);
	drivePin(GPIO_Pin_4|GPIO_Pin_5);
	delay_us(250);
	startConversion();
	while(!trasferComplete);
	rawY=ADC_results[1];	
}

int detectAndRead(){
	if(detect_touch()){
		readXY();
		return 1;
	}else{
		return 0;
	}
}

int main(void)
{
	int lastState=0;
  /* Add your application code here
     */	
	setToMaxSpeed();
	configureUART2();
	configureSPLYPins();
	printf("Configurando ADC...");
	configureADC();
	NVIC_EnableIRQ(DMA1_Ch1_IRQn);
	printf("\tcompletado\n");
  //pruebaChar();
	printf("Iniciando GLCD...");
	glcd_init(GRAPHIC);
	glcd_fill_screen(OFF);
	drawPushButton2("Hola Mundo",10,10,OFF,OFF);
	glcd_load_buffer();
	printf("\tcompletado \n");
	while(1){
	//  printf("Iniciando conversion...");
		while(1){
			if(detectAndRead()){
				if(!lastState){
					glcd_fill_screen(OFF);
					drawPushButton2("Hola Mundo",10,10,OFF,ON);
					glcd_load_buffer();
				}
				break;
			}else{
				if(lastState){
					printf("touch not detected\n");
					glcd_fill_screen(OFF);
					drawPushButton2("Hola Mundo",10,10,OFF,OFF);
					glcd_load_buffer();
				}
				lastState=0;
			}
			delay_ms(25);
		}
		lastState=1;
		printf("X=%d\tY=%d\n",rawX,rawY);
		//printf("cycle complete\n");
		delay_ms(25);
	}
}

void drawPushButton(char *a,unsigned char x, unsigned char y, unsigned char color,unsigned char state){
    char c;
    int i=0;
    while((c=a[i])!='\0')i++;
    if(!state) glcd_rect(x-3,y-3,x+(i*6)+1,y+8+1,YES,!color);
	  if(state) glcd_rect(x-4,y-4,x+(i*6)+2,y+8+2,YES,!color);
    glcd_text57(x,y,a,1,color);
		if(!state) glcd_rect(x-3,y-3,x+(i*6)+1,y+8+1,NO,color);
	  if(state) glcd_rect(x-4,y-4,x+(i*6)+2,y+8+2,NO,color);
	
		if(!state) glcd_rect(x-4,y-4,x+(i*6)+2,y+8+2,NO,!color);
		if(state) glcd_rect(x-5,y-5,x+(i*6)+3,y+8+3,NO,!color);
}

void drawPushButton2(char *a,unsigned char x, unsigned char y, unsigned char color,unsigned char state){
    char c;
    int i=0;
    while((c=a[i])!='\0')i++;
		glcd_rect(x-3,y-3,x+(i*6)+1,y+8+1,YES,!state);
    glcd_text57(x,y,a,1,state);
		glcd_rect(x-3,y-3,x+(i*6)+1,y+8+1,NO,state);
	  glcd_rect(x-4,y-4,x+(i*6)+2,y+8+2,NO,!color);
}

void boton1Callback(int event_code,void* item_data){
	
}

void configureADC(){

	ADC_InitTypeDef myADC_Init;
	DMA_InitTypeDef myDMA_Init;
	//Configure PB0 and PB1 as analog input
  GPIO_InitTypeDef myAnalogPin;
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);//Turn on Port B
	ADC_StructInit(&myADC_Init);
	myAnalogPin.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//configure PB0 and PB1 at the same time
	myAnalogPin.GPIO_Mode=GPIO_Mode_AN;//Pins as anlog pins
	GPIO_Init(GPIOA,&myAnalogPin);//init gpio port with configuration
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//Turn on peripherial ADC1
	/*Calibrate ADC*/
	ADC_Cmd(ADC1,DISABLE);
	ADC_GetCalibrationFactor(ADC1);
	ADC_DMARequestModeConfig(ADC1,ADC_DMAMode_OneShot);
	ADC_DMACmd(ADC1,ENABLE);
	//
	
	//Configure Init Structure
	ADC_StructInit(&myADC_Init);
	myADC_Init.ADC_ScanDirection=ADC_ScanDirection_Upward;
	myADC_Init.ADC_DataAlign=ADC_DataAlign_Right;
	myADC_Init.ADC_Resolution=ADC_Resolution_10b;
	myADC_Init.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_Init(ADC1,&myADC_Init);
	ADC_Cmd(ADC1,ENABLE);
	//
	ADC_ChannelConfig(ADC1,ADC_Channel_8,ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1,ADC_Channel_9,ADC_SampleTime_239_5Cycles);
	//Enable EndOfConverison Interrupts generation
	//ADC_WaitModeCmd(ADC1,ENABLE);
	
	//
	//ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_StructInit(&myDMA_Init);
	myDMA_Init.DMA_M2M=DMA_M2M_Disable;
	myDMA_Init.DMA_Priority=DMA_Priority_Medium;
	myDMA_Init.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	myDMA_Init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	myDMA_Init.DMA_MemoryInc=DMA_MemoryInc_Enable;
	myDMA_Init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	myDMA_Init.DMA_Mode=DMA_Mode_Normal;
	myDMA_Init.DMA_DIR=DMA_DIR_PeripheralSRC;
	myDMA_Init.DMA_MemoryBaseAddr=(unsigned int)&ADC_results;
	myDMA_Init.DMA_PeripheralBaseAddr=(unsigned int)(&(ADC1->DR));
	myDMA_Init.DMA_BufferSize=2;

	DMA_Init(DMA1_Channel1,&myDMA_Init);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
}



void startConversion(void){
	trasferComplete=0;
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,2);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_StartOfConversion(ADC1);
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

void DMA1_Channel1_IRQHandler(void){
	DMA_ClearITPendingBit(DMA_IT_TC);//clear trasfer complete flag
	trasferComplete=1;
} 
