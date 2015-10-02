#include <avr/io.h>
#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)

int main(void)
{
	unsigned int pressure; // Variable to hold ADC result

	DDRD = 0b11111111;//0xff; // Set Port D as Output
	PORTD = 0b00000000;//0x00;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	//ADPS2: ADPS2 and ADPS0 set to make division factor 32
	ADMUX = 0b00000101;//0x05; // ADC input channel set to PC5

	while (1)
	{
		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure = ADCW; //Store ADC value

		if (pressure < 512)
		{
			PORT_OFF(PORTD,7); // Toggle LEDs
			PORT_ON (PORTD,6);
		}
		else
		{
			PORT_ON(PORTD,7); // Toggle LEDs
			PORT_OFF (PORTD,6);
		}
	}
}
