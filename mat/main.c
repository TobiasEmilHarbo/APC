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
int dangerLvlOne, dangerLvlTwo, dangerLvlThree;

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
	ADMUX = byte; // ADC input channel set
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
	}
}

void readDangerLvl()
{
	if(dangerLvlTwo > 0 || dangerLvlOne > 1) //Level 2
	{
		uart_transmitByte('2');
		_delay_ms(5500);
	}
	else if(dangerLvlOne > 0) //Level 1
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
	DDRD  = 0b11111101; // Set Port D - RX as Output
	PORTD = 0b00000010;

	DDRB = 0b11111111; // Set Port B as Output
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	// ADPS2: ADPS2 and ADPS0 set to make division factor 32

	uart_init();

	int baseThreshold = 500;

	int adjustThresholdInnerA = -105;
	int adjustThresholdInnerB = -133;
	int adjustThresholdInnerC = 147;
	int adjustThresholdInnerD = -289;

	int adjustThresholdOuterA = -220;
	int adjustThresholdOuterB = -199;
	int adjustThresholdOuterC = 107;
	int adjustThresholdOuterD = -353;

	_delay_ms(500);

	while(1)
	{
		dangerLvlOne 	= 0;
		dangerLvlTwo 	= 0;

		pressureA = readPressure(0b00000000); //PC0
		pressureB = readPressure(0b00000001); //PC1
		pressureC = readPressure(0b00000010); //PC2
		pressureD = readPressure(0b00000011); //PC3

		/* ==== pressure plate A ==== */

		if (pressureA > (baseThreshold + adjustThresholdInnerA))
		{
			PORT_ON(PORTB,0); //debug
			PORT_ON(PORTD,7); //debug

			setLvlOfDanger(2);
		}
		else if (pressureA > (baseThreshold + adjustThresholdOuterA))
		{
			PORT_ON(PORTB,0); //debug
			PORT_OFF(PORTD,7); //debug

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTB,0); //debug
			PORT_OFF(PORTD,7); //debug

			setLvlOfDanger(0);
		}

/* ==== pressure plate B ==== */


		if (pressureB > (baseThreshold + adjustThresholdInnerB))
		{
			PORT_ON(PORTD,6); //debug
			PORT_ON(PORTD,5); //debug

			setLvlOfDanger(2);
		}
		else if (pressureB > (baseThreshold + adjustThresholdOuterB))
		{
			PORT_ON(PORTD,6); //debug
			PORT_OFF(PORTD,5); //debug

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTD,6); //debug
			PORT_OFF(PORTD,5); //debug

			setLvlOfDanger(0);
		}

/* ==== pressure plate C ==== */

		if (pressureC > (baseThreshold + adjustThresholdInnerC))
		{
			PORT_ON(PORTB,7);
			PORT_ON(PORTB,6);

			setLvlOfDanger(2);
		}
		else if (pressureC > (baseThreshold + adjustThresholdOuterC))
		{
			PORT_ON(PORTB,7);
			PORT_OFF(PORTB,6);

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

			setLvlOfDanger(2);
		}
		else if (pressureD > (baseThreshold + adjustThresholdOuterD))
		{
			PORT_ON(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);

			setLvlOfDanger(1);
		}
		else
		{
			PORT_OFF(PORTB,1); // Toggle LEDs
			PORT_OFF(PORTB,2);

			setLvlOfDanger(0);
		}

		readDangerLvl();
	}
}
