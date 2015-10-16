#define F_CPU 4000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#include <avr/io.h>
#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)
typedef unsigned char BYTE;

unsigned int pressureA, pressureB, pressureC, pressureD; // Variable to hold ADC result
int lvlOfDanger, dangerLvlOne, dangerLvlTwo, dangerLvlThree;

void uart_transmitByte (unsigned char);
void uart_receiveByte (void);

int readPressure(BYTE byte)
{
	ADMUX = byte;//0x05; // ADC input channel set to PC5
	ADCSRA |= (1<<ADSC); // Start conversion
	while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

	return ADCW;
}

void setLvlOfDanger(int lvl)
{
	switch(lvl)
	{
		case 1:
			dangerLvlOne++;
			break;
		case 2:
			dangerLvlTwo++;
			break;
		case 3:
			dangerLvlThree++;
			break;
	}

	//if(lvlOfDanger < lvl)
	//	lvlOfDanger = lvl;
}

void readDangerLvl()
{

	/*if(dangerLvlThree > 0 || dangerLvlTwo > 1)
	{
		PORT_ON(PORTD,1);
	}
	else if(dangerLvlTwo > 0)
	{
		PORT_ON(PORTD,2);
	}
	else if(dangerLvlThree > 0)
	{
		PORT_ON(PORTD,3);
	}*/
	if(dangerLvlThree > 0 || dangerLvlTwo > 1)
	{
		PORT_ON(PORTD,1);
	}
	else if(dangerLvlTwo > 0)
	{
		PORT_ON(PORTD,2);
	}
	else if(dangerLvlOne > 0)
	{
		PORT_ON(PORTD,3);
	}
	else
	{
		PORT_ON(PORTD,4);
	}

	/*switch(lvlOfDanger)
	{
		case 0:
			PORT_ON(PORTD,4);
		break;
		case 1:
			PORT_ON(PORTD,3);
		break;
		case 2:
			PORT_ON(PORTD,2);
		break;
		case 3:
			PORT_ON(PORTD,1);
		break;
	}*/
}

int main(void)
{
	DDRD  = 0b11111101; // Set Port D as Output
	PORTD = 0b00000010;

	DDRB = 0b11111111; // Set Port B as Output
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	// ADPS2: ADPS2 and ADPS0 set to make division factor 32

	int baseThreshold = 500;

	int adjustThresholdTopA = -38;
	int adjustThresholdTopB = -127;
	int adjustThresholdTopC = 93;
	int adjustThresholdTopD = -207;

	int adjustThresholdMidA = -125;
	int adjustThresholdMidB = -185;
	int adjustThresholdMidC = 29;
	int adjustThresholdMidD = -270;

	/* === not used === */
	int adjustThresholdBotA = 900;
	int adjustThresholdBotB = 900;
	int adjustThresholdBotC = 900;
	int adjustThresholdBotD = 900;

	while (1)
	{
		lvlOfDanger 	= 0;
		dangerLvlOne 	= 0;
		dangerLvlTwo 	= 0;
		dangerLvlThree 	= 0;

		pressureA = readPressure(0b00000000); //PC0
		pressureB = readPressure(0b00000001); //PC1
		pressureC = readPressure(0b00000010); //PC2
		pressureD = readPressure(0b00000011); //PC3

		//PORT_ON(PORTD,4);
		//PORT_ON(PORTD,3);
		//PORT_ON(PORTD,2);
		//PORT_ON(PORTD,1);

/* ==== pressure plate A ==== */

		if (pressureA > (baseThreshold + adjustThresholdTopA))
		{
			PORT_ON(PORTB,0);
			PORT_ON(PORTD,7);

			setLvlOfDanger(3);
		}
		else if (pressureA > (baseThreshold + adjustThresholdMidA))
		{
			PORT_ON(PORTB,0);
			PORT_OFF(PORTD,7);

			setLvlOfDanger(2);
		}
		else if (pressureA > (baseThreshold + adjustThresholdBotA))
		{
			PORT_OFF(PORTB,0);
			PORT_ON(PORTD,7);

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTB,0);
			PORT_OFF(PORTD,7);

			setLvlOfDanger(0);
		}

/* ==== pressure plate B ==== */


		if (pressureB > (baseThreshold + adjustThresholdTopB))
		{
			PORT_ON(PORTD,6);
			PORT_ON(PORTD,5);

			setLvlOfDanger(3);
		}
		else if (pressureB > (baseThreshold + adjustThresholdMidB))
		{
			PORT_ON(PORTD,6);
			PORT_OFF(PORTD,5);

			setLvlOfDanger(2);
		}
		else if (pressureB > (baseThreshold + adjustThresholdBotB))
		{
			PORT_OFF(PORTD,6);
			PORT_ON(PORTD,5);

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTD,6); // Toggle LEDs
			PORT_OFF(PORTD,5);

			setLvlOfDanger(0);
		}

/* ==== pressure plate C ==== */

		if (pressureC > (baseThreshold + adjustThresholdTopC))
		{
			PORT_ON(PORTB,7);
			PORT_ON(PORTB,6);

			setLvlOfDanger(3);
		}
		else if (pressureC > (baseThreshold + adjustThresholdMidC))
		{
			PORT_ON(PORTB,7);
			PORT_OFF(PORTB,6);

			setLvlOfDanger(2);
		}
		else if (pressureC > (baseThreshold + adjustThresholdBotC))
		{
			PORT_OFF(PORTB,7);
			PORT_ON(PORTB,6);

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTB,7); // Toggle LEDs
			PORT_OFF(PORTB,6);

			setLvlOfDanger(0);
		}

/* ==== pressure plate D ==== */

		if (pressureD > (baseThreshold + adjustThresholdTopD))
		{
			PORT_ON(PORTB,1);
			PORT_ON(PORTB,2);

			setLvlOfDanger(3);
		}
		else if (pressureD > (baseThreshold + adjustThresholdMidD))
		{
			PORT_ON(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);

			setLvlOfDanger(2);
		}
		else if (pressureD > (baseThreshold + adjustThresholdBotD))
		{
			PORT_OFF(PORTB,1); // Toggle LEDs
			PORT_ON(PORTB,2);

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);

			setLvlOfDanger(0);
		}

		PORT_OFF(PORTD,1);
		PORT_OFF(PORTD,2);
		PORT_OFF(PORTD,3);
		PORT_OFF(PORTD,4);

		readDangerLvl();
	}
}
