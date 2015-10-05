#define F_CPU 8000000UL 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>                                    // set the CPU clock

void receiveData(void);
void transmitData(void);

void uart_transmit (unsigned char);
unsigned char uart_receive (void);

// define some macros
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
  
// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN||1<<RXCIE);                // enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

// function to send data
void uart_transmit (unsigned char data)
{
    while (!( UCSRA & (1<<UDRE)));                // wait while register is free
    UDR = data;                                   // load data in the register
}

// function to receive data
unsigned char uart_receive (void)
{
        while(!(UCSRA) & (1<<RXC));                     // wait while data is being received
        return UDR;                                     // return 8-bit data
}

int main (void)
{

	uart_init();
	
	transmitData();
}

void receiveData (void)
{
	
	unsigned char data;
	sei();

	DDRC=0xFF;//make PORTC output
	PORTC=0x00;//set all outputs to 0

	while(1)
	{

		data = uart_receive;

		if(data=='1'){
			PORTC=0b00100000;//turn on LED if recieved character is 'a'
		}
		else 
		{	
			PORTC=0b00000000;	
		}
	}
}

void transmitData (void)
{
	while(1)
	{
		_delay_ms(5000);
		uart_transmit('1');
	}
}