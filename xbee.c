#define F_CPU 8000000UL 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>                                    // set the CPU clock

//Method declarations
void receiveData(void); 
void transmitData(void);

void uart_transmit (unsigned char);
unsigned char uart_receive (void);

// define baudrate for serial communication
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR
  
unsigned char data;

// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}


void uart_transmit (unsigned char data)
{
    while (!( UCSRA & (1<<UDRE)));              // wait while register is free
    UDR = data;                                 // load data in the register
}


unsigned char uart_receive (void)
{
        while(!(UCSRA) & (1<<RXC));             // wait while data is being received
        return UDR;                             // return 8-bit data
}

int main (void)
{
	DDRC=0xFF;									//make PORTC output
	PORTC=0x00;									//set all outputs to 0
	
	uart_init();
	
	while(1){
		receiveData();
		transmitData();
	}
}

void receiveData (void)
{

	data = uart_receive();

	if(data=='1'){
		PORTC=0b00100000;						//turn on LED if recieved character is 'a'
	}
	else 
	{	
		PORTC=0b00000000;	
	}
}



void transmitData (void)
{
	uart_transmit('1');
	_delay_ms(1000);
}