#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"
#include "adc.h"
#include "mq2.h"

uint16_t ReadADC(uint8_t ADCchannel);

float Ro=10;                 //Ro is initialized to 10 kilo ohms
float ph=0;

int main(){
	unsigned char Res[16], lpg[16], smoke[16];
	char lcddata[16];
	lcdinit();
	adcinit();
	DDRB = 0b00000001;
	PORTB = 0b00000000;
 	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));   // prescaler 128
 	ADMUX |= (1<<REFS0);					//external vcc
	ADCSRA |= (1<<ADEN);                            // Turn on ADC
// 	DDRD=0b1;
	lcdstr("PH ...");
	//Ro = SensorCalibration();                       //Please make sure the sensor is in clean air when you perform the calibration
	dtostrf(Ro, 6, 2, Res);
	lcdclear();
	_delay_ms(2);	//clearing takes around 1.64ms to execute
	
	//lcdgoto(1,2);
	//lcdstr("SMOKE:");
	//lcdgoto(13,2);
	//lcdstr("PPM");
	
	while (1){
		//itoa(GetGasPercentage(ReadSensor()/Ro,LPG), lpg, 10);
		
		ph=ReadADC(0);
		ph = ph * 5.0 / 1024 * 3.5;
		
		
		
		itoa(ph,lcddata,10);
		
		lcdclear();
		_delay_ms(2);
		if (ph < 6.5) {
			lcdstr("Low PH");
		}
		if (6.5 <= ph && ph <= 7.5){
			lcdstr("Normal PH");
		}
		if (7.5 < ph) {
			lcdstr("High PH");
		}
		
		lcdgoto(13,2);
		lcdstr(lcddata);
		_delay_ms(500);
		
		
// 		if(GetGasPercentage(ReadSensor()/Ro,SMOKE)>1500)
// 		{
// 			PORTB = 0b00000001;
// 		}
// 		else
// 		{
// 			PORTB = 0b00000000;
// 		}
		
		

	}
	return 0;
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


