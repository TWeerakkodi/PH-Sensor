


#define F_CPU 8000000
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "LCDI2C.h"



uint16_t ReadADC(uint8_t ADCchannel);
float ph=0;
char lcddata[20];
int main(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
	ADMUX |= (1<<REFS0);					//external vcc
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
	LcdInit();
	LcdSetCursor(0,0,"Wait . . .");
	_delay_ms(5000);
	LcdCommand(LCD_CLEARDISPLAY);
	ph=ReadADC(0)*5.0/1024;
	sprintf(lcddata,"%0.2f",ph*3.5);
	LcdSetCursor(0,0,lcddata);
	DDRD=0b1;
	while (1)
	{
		LcdCommand(LCD_CLEARDISPLAY);
		ph=ReadADC(0)*5.0/1024;
		sprintf(lcddata,"%0.2f",ph*3.5);
		LcdSetCursor(0,0,lcddata);
		DDRD=0b1;
	}
}

uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADCW;
}
