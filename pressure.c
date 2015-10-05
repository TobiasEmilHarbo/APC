#include <avr/io.h>
#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)

int main(void)
{
	//unsigned int pressure1; // Variable to hold ADC result
	//unsigned int pressure2; // Variable to hold ADC result

	uint16_t pressure1, pressure2;

	DDRD = 0b11111111;//0xff; // Set Port D as Output
	PORTD = 0b00000000;//0x00;
	DDRB = 0b11111111;
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	//ADPS2: ADPS2 and ADPS0 set to make division factor 32


	while (1)
	{
		ADMUX = 0b00000100;//0x04; // ADC input channel set to PC4

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure1 = ADCW; //Store ADC value

		ADMUX = 0b00000101; //0x05; ADC input channel set to PC5

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure2 = ADCW;


		if (pressure1 < 800)
		{
			PORT_OFF(PORTD,7); // Toggle LEDs
			PORT_ON (PORTD,6);
		}
		else
		{
			PORT_ON(PORTD,7); // Toggle LEDs
			PORT_OFF (PORTD,6);
		}

		if(pressure2 < 800)
		{
			PORT_OFF(PORTB,1);
			PORT_ON(PORTB,2);
		}
		else
		{
			PORT_ON(PORTB,1);
			PORT_OFF(PORTB,2);
		}
	}
}
