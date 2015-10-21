//define the clockrate to fit the ATmega8L
#define F_CPU 4000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

typedef unsigned char BYTE;

//libraries
#include <avr/io.h>
#include <util/delay.h>

unsigned int pressureA, pressureB, pressureC, pressureD; // Variable to hold ADC result
int dangerLvlOne, dangerLvlTwo, dangerLvlThree;
unsigned char receivedData;

void uart_transmitByte (unsigned char);
void uart_receiveByte (void);
void uart_Flush (void);

// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

//function to transmit an unsigned char via XBees
void uart_transmitByte (unsigned char transmitterData)
{
    while (!(UCSRA & (1<<UDRE)));              // wait while register is free
    UDR = transmitterData;                     // load data in the register
    uart_Flush();
}

//function to recieve bytes via XBees
void uart_receiveByte (void)
{
	while(!(UCSRA) & (1<<RXC));           	   // wait while data is being received
	receivedData = UDR;                        // return 8-bit data
}

//function to clean out the uart
void uart_Flush(void)
{
	unsigned char dummy;
	while ( UCSRA & (1<<RXC) ) dummy = UDR;
}

//read the pressure from a given port on the ATmega8L
int readPressure(BYTE byte)
{
	ADMUX = byte; // ADC input channel set to parsed byte
	ADCSRA |= (1<<ADSC); // Start conversion
	while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

	return ADCW;
}

//set the level of danger determined by how high the pressure is
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

//read the danger level and transmit to the bracelet via XBees
void transmitDangerLvl()
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
	DDRD  = 0b11111101; // Set Port D (leaving out RX) as output
	PORTD = 0b00000010;

	DDRB = 0b11111111; // Set Port B as Output
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	// ADPS2: ADPS2 and ADPS0 set to make division factor 32

	uart_init();

/* === Pressure sensors calibration === */

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
		//reset danger levels before each readings
		dangerLvlOne 	= 0;
		dangerLvlTwo 	= 0;

		//read each pressure sensor
		pressureA = readPressure(0b00000000); //PC0
		pressureB = readPressure(0b00000001); //PC1
		pressureC = readPressure(0b00000010); //PC2
		pressureD = readPressure(0b00000011); //PC3

	/* ==== pressure plate A ==== */

		if (pressureA > (baseThreshold + adjustThresholdInnerA))
		{
			setLvlOfDanger(2);
		}
		else if (pressureA > (baseThreshold + adjustThresholdOuterA))
		{
			setLvlOfDanger(1);
		}
		else
		{
			setLvlOfDanger(0);
		}

	/* ==== pressure plate B ==== */

		if (pressureB > (baseThreshold + adjustThresholdInnerB))
		{
			setLvlOfDanger(2);
		}
		else if (pressureB > (baseThreshold + adjustThresholdOuterB))
		{
			setLvlOfDanger(1);
		}
		else
		{
			setLvlOfDanger(0);
		}

	/* ==== pressure plate C ==== */

		if (pressureC > (baseThreshold + adjustThresholdInnerC))
		{
			setLvlOfDanger(2);
		}
		else if (pressureC > (baseThreshold + adjustThresholdOuterC))
		{
			setLvlOfDanger(1);
		}
		else
		{
			setLvlOfDanger(0);
		}

	/* ==== pressure plate D ==== */

		if (pressureD > (baseThreshold + adjustThresholdInnerD))
		{
			setLvlOfDanger(2);
		}
		else if (pressureD > (baseThreshold + adjustThresholdOuterD))
		{
			setLvlOfDanger(1);
		}
		else
		{
			setLvlOfDanger(0);
		}

		//transmit the danger level to bracelet via XBees
		transmitDangerLvl();
	}
}
