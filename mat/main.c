#define F_CPU 4000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#include <avr/io.h>
#include <util/delay.h>

#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)
typedef unsigned char BYTE;

unsigned int pressureA, pressureB, pressureC, pressureD; // Variable to hold ADC result
int lvlOfDanger, dangerLvlOne, dangerLvlTwo, dangerLvlThree;

void uart_transmitByte (unsigned char);
void uart_receiveByte (void);
void uart_Flush (void);
unsigned char receivedData;

// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void uart_transmitByte (unsigned char transmitterData)
{
    while (!(UCSRA & (1<<UDRE)));              // wait while register is free
    UDR = transmitterData;                                 // load data in the register
    uart_Flush();
}

void uart_receiveByte (void)
{
	while(!(UCSRA) & (1<<RXC));             		// wait while data is being received
	receivedData = UDR;                             // return 8-bit data
}

void uart_Flush(void)
{
	unsigned char dummy;
	while ( UCSRA & (1<<RXC) ) dummy = UDR;
}

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
}

void readDangerLvl()
{
	if(dangerLvlThree > 0 || dangerLvlTwo > 1) //Level 2
	{
		uart_transmitByte('2');
		_delay_ms(5500);
	}
	else if(dangerLvlTwo > 0) //Level 1
	{
		uart_transmitByte('1');
		_delay_ms(5500);
	}
	else //Neutral
	{
		uart_transmitByte('0');
		_delay_ms(100);
	}
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

	uart_init();

	int baseThreshold = 500;

	int adjustThresholdInnerA = -53;
	int adjustThresholdInnerB = -133;
	int adjustThresholdInnerC = 102;
	int adjustThresholdInnerD = -250;

	int adjustThresholdMidA = -150;
	int adjustThresholdMidB = -199;
	int adjustThresholdMidC = 13;
	int adjustThresholdMidD = -305;

	/* === not used === */
	int adjustThresholdOuterA = 1023;
	int adjustThresholdOuterB = 1023;
	int adjustThresholdOuterC = 1023;
	int adjustThresholdOuterD = 1023;

	_delay_ms(500);

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

		if (pressureA > (baseThreshold + adjustThresholdInnerA))
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
		else if (pressureA > (baseThreshold + adjustThresholdOuterA))
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


		if (pressureB > (baseThreshold + adjustThresholdInnerB))
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
		else if (pressureB > (baseThreshold + adjustThresholdOuterB))
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

		if (pressureC > (baseThreshold + adjustThresholdInnerC))
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
		else if (pressureC > (baseThreshold + adjustThresholdOuterC))
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

		if (pressureD > (baseThreshold + adjustThresholdInnerD))
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
		else if (pressureD > (baseThreshold + adjustThresholdOuterD))
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
