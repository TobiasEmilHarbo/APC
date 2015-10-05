#define F_CPU 8000000UL 

#include <util/delay.h>
#include <avr/io.h>
#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)

// define baudrate for serial communication
#define BAUD 9600                               // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)        // set baud rate value for UBRR

void transmitData(unsigned char);
void uart_transmit (unsigned char);

unsigned char data;

void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void uart_transmit (unsigned char data)
{
    while (!(UCSRA & (1<<UDRE)));              // wait while register is free
    UDR = data;                                 // load data in the register
}

void transmitData (unsigned char data)
{
	uart_transmit(data);

}

int main(void)
{
	
	uart_init();

	//unsigned int pressure1; // Variable to hold ADC result
	//unsigned int pressure2; // Variable to hold ADC result

	uint16_t pressure1, pressure2, pressure3, pressure4;

	DDRD = 0b11111111;//0xff; // Set Port D as Output
	PORTD = 0b00000000;//0x00;
	DDRB = 0b11111111;
	PORTB = 0b00000000;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
	// ADEN: Set to turn on ADC , by default it is turned off
	//ADPS2: ADPS2 and ADPS0 set to make division factor 32


	while (1)
	{
		// The four sensors uses PC1, PC2, PC3 and PC4
		ADMUX = 0b00000001;//0x01; // ADC input channel set to PC1

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure1 = ADCW; //Store ADC value

		ADMUX = 0b00000010; //0x02; ADC input channel set to PC2

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure2 = ADCW;

		ADMUX = 0b00000011;//0x03; // ADC input channel set to PC3

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure3 = ADCW; //Store ADC value

		ADMUX = 0b00000100; //0x04; ADC input channel set to PC4

		ADCSRA |= (1<<ADSC); // Start conversion
		while (ADCSRA & (1<<ADSC)); // wait for conversion to complete

		pressure4 = ADCW;


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
			transmitData(1);
		}
		else
		{
			PORT_ON(PORTB,1);
			PORT_OFF(PORTB,2);
			transmitData(0);

		}
		/*//TODO: Decide what ports to use for the LED
		if (pressure3 < 800)
		{
			PORT_OFF(PORTD,0); // Toggle LEDs
			PORT_ON(PORTD,1);
		}
		else
		{
			PORT_ON(PORTD,0); // Toggle LEDs
			PORT_OFF (PORTD,1);
		}*/
		if(pressure4 < 800)
		{
			PORT_OFF(PORTD,2);
			PORT_ON(PORTD,3);
		}
		else
		{
			PORT_ON(PORTD,2);
			PORT_OFF(PORTD,3);
		}

	}
}
