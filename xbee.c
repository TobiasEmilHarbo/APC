#define F_CPU 8000000UL 

#include <util/delay.h>
#include <avr/io.h>

//Method declarations
void receiveData(void); 
unsigned char uart_receiveByte (void);

void transmitData(unsigned char);
void uart_transmitByte (unsigned char);

// define baudrate for serial communication
#define BAUD 9600                               // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)        // set baud rate value for UBRR
  
unsigned char data;

// function to initialize UART
void uart_init (void)
{
    UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
    UBRRL = BAUDRATE;                           // set baud rate
    UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void uart_transmitByte (unsigned char data)
{
    while (!(UCSRA & (1<<UDRE)));              // wait while register is free
    UDR = data;                                 // load data in the register
}

unsigned char uart_receiveByte (void)
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
		//transmitData();
		_delay_ms(500);
	}
}

void receiveData (void)
{
	data = uart_receiveByte();

	if(data=='1'){
		PORTC=0b00100000;						//turn on LED if received character is '1'
	}
	else 
	{	
		PORTC=0b00000000;	
	}
}

void transmitData (unsigned char data)
{
	uart_transmitByte(data);
}
