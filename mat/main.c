#include <avr/io.h>
#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)
typedef unsigned char BYTE;

unsigned int pressureA, pressureB, pressureC, pressureD; // Variable to hold ADC result

int readPressure(BYTE byte)
{
	ADMUX = byte;//0x05; // ADC input channel set to PC5
	ADCSRA |= (1<<ADSC); // Start conversion
	while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

	return ADCW;
}

int main(void)
{
	DDRD = 0b11111111; // Set Port D as Output
	PORTD = 0b00000000;

	DDRB = 0b11111111; // Set Port B as Output
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	// ADPS2: ADPS2 and ADPS0 set to make division factor 32

	int baseThreshold = 600;

	int adjustThresholdTopA = 0;
	int adjustThresholdTopB = 0;
	int adjustThresholdTopC = 0;
	int adjustThresholdTopD = 0;

	int adjustThresholdMidA = -150;
	int adjustThresholdMidB = -150;
	int adjustThresholdMidC = -150;
	int adjustThresholdMidD = -150;

	int adjustThresholdBotA = -280;
	int adjustThresholdBotB = -280;
	int adjustThresholdBotC = -280;
	int adjustThresholdBotD = -280;

	while (1)
	{
		pressureA = readPressure(0b00000000); //PC0
		pressureB = readPressure(0b00000001); //PC1
		pressureC = readPressure(0b00000010); //PC2
		pressureD = readPressure(0b00000011); //PC3

/* ==== pressure plate A ==== */

		if (pressureA > (baseThreshold + adjustThresholdTopA))
		{
			PORT_ON(PORTB,0);
			PORT_ON(PORTD,7);
		}
		else if (pressureA > (baseThreshold + adjustThresholdMidA))
		{
			PORT_ON(PORTB,0);
			PORT_OFF(PORTD,7);
		}
		else if (pressureA > (baseThreshold + adjustThresholdBotA))
		{
			PORT_OFF(PORTB,0);
			PORT_ON(PORTD,7);
		}
		else
		{
			PORT_OFF(PORTB,0);
			PORT_OFF(PORTD,7);
		}

/* ==== pressure plate B ==== */


		if (pressureB > (baseThreshold + adjustThresholdTopB))
		{
			PORT_ON(PORTD,6);
			PORT_ON(PORTD,5);
		}
		else if (pressureB > (baseThreshold + adjustThresholdMidB))
		{
			PORT_ON(PORTD,6);
			PORT_OFF(PORTD,5);
		}
		else if (pressureB > (baseThreshold + adjustThresholdBotB))
		{
			PORT_OFF(PORTD,6);
			PORT_ON(PORTD,5);
		}
		else
		{
			PORT_OFF(PORTD,6); // Toggle LEDs
			PORT_OFF(PORTD,5);
		}

/* ==== pressure plate C ==== */

		if (pressureC > (baseThreshold + adjustThresholdTopC))
		{
			PORT_ON(PORTB,7);
			PORT_ON(PORTB,6);
		}
		else if (pressureC > (baseThreshold + adjustThresholdMidC))
		{
			PORT_ON(PORTB,7);
			PORT_OFF(PORTB,6);
		}
		else if (pressureC > (baseThreshold + adjustThresholdBotC))
		{
			PORT_OFF(PORTB,7);
			PORT_ON(PORTB,6);
		}
		else
		{
			PORT_OFF(PORTB,7); // Toggle LEDs
			PORT_OFF(PORTB,6);
		}

/* ==== pressure plate D ==== */

		if (pressureD > (baseThreshold + adjustThresholdTopD))
		{
			PORT_ON(PORTB,1);
			PORT_ON(PORTB,2);
		}
		else if (pressureD > (baseThreshold + adjustThresholdMidD))
		{
			PORT_ON(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);
		}
		else if (pressureD > (baseThreshold + adjustThresholdBotD))
		{
			PORT_OFF(PORTB,1); // Toggle LEDs
			PORT_ON(PORTB,2);
		}
		else
		{
			PORT_OFF(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);
		}
	}
}
