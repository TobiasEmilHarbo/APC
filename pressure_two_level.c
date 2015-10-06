#include <avr/io.h>
#define PORT_OFF(port,pin) port |= (1<<pin)
#define PORT_ON(port,pin) port &= ~(1<<pin)
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

	int baseThreshold = 900;

	int adjustThresholdA = 0;
	int adjustThresholdB = 0;
	int adjustThresholdC = 100;
	int adjustThresholdD = 50;

	int adjustBottomThresholdA = 100;
	int adjustBottomThresholdB = 100;
	int adjustBottomThresholdC = 0;
	int adjustBottomThresholdD = 100;

	while (1)
	{
		pressureA = readPressure(0b00000000); //PC0
		pressureB = readPressure(0b00000001); //PC1
		pressureC = readPressure(0b00000010); //PC2
		pressureD = readPressure(0b00000011); //PC3

	/* ==== A ==== */

		PORT_ON(PORTD,7);
		PORT_ON(PORTB,0);
		PORT_OFF(PORTD,7);
		PORT_OFF(PORTB,0);

		if (pressureA > (baseThreshold + adjustThresholdA))
		{
			PORT_ON(PORTD,7);
			PORT_ON(PORTB,0);
		}
		else if (pressureA > ((baseThreshold + adjustThresholdA) - adjustBottomThresholdA))
		{
			PORT_ON(PORTD,7);
			PORT_OFF(PORTB,0);
		}

	/* ==== B ==== */

		PORT_ON(PORTD,5);
		PORT_ON(PORTD,6);
		PORT_OFF(PORTD,5);
		PORT_OFF(PORTD,6);

		if (pressureB > (baseThreshold + adjustThresholdB))
		{
			PORT_ON(PORTD,6);
			PORT_ON(PORTD,5);
		}
		else if (pressureB > ((baseThreshold + adjustThresholdB) - adjustBottomThresholdB))
		{
			PORT_OFF(PORTD,6); // Toggle LEDs
			PORT_ON(PORTD,5);
		}

	/* ==== C ==== */

		PORT_ON(PORTB,7); //red
		PORT_ON(PORTB,6); //blue
		PORT_OFF(PORTB,7);
		PORT_OFF(PORTB,6);

		if (pressureC > (baseThreshold + adjustThresholdC))
		{
			PORT_ON(PORTB,6);
			PORT_ON(PORTB,7);
		}
		else if (pressureC > ((baseThreshold + adjustThresholdC) - adjustBottomThresholdC))
		{
			PORT_OFF(PORTB,7); // Toggle LEDs
			PORT_ON(PORTB,6);
		}

	/* ==== D ==== */

		PORT_ON(PORTB,1);
		PORT_ON(PORTB,2);
		PORT_OFF(PORTB,1);
		PORT_OFF(PORTB,2);

		if (pressureD > (baseThreshold + adjustThresholdD))
		{
			PORT_ON(PORTB,2);
			PORT_ON(PORTB,1);
		}
		else if (pressureD > ((baseThreshold + adjustThresholdD) - adjustBottomThresholdD))
		{
			PORT_ON(PORTB,2); // Toggle LEDs
			PORT_OFF(PORTB,1);
		}
	}
}
