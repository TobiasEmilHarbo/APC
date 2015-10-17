#define F_CPU 4000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//Method declarations
void uart_transmitByte (unsigned char);
void uart_receiveByte (void);

typedef unsigned char BYTE;
int readSignalStrength(BYTE byte);

unsigned char data, i, receivedData, message[10];
int prevPulse = 0;	

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

void uart_receiveByte (void)
{
	while(!(UCSRA) & (1<<RXC));             		// wait while data is being received
	receivedData = UDR;                             // return 8-bit data
}

void StartTimer1(void)
{
	TCNT1=0;
	//Start timer with prescaller 1024
	TCCR1B = (1<<CS12)|(0<<CS11)|(1<<CS10);
}

void StopTimer1(void)
{
	TCCR1B = (0<<CS12)|(0<<CS11)|(0<<CS10);
	
	if(TCNT1 > 10000){
		uart_transmitByte('c');	
	}
}

int main( void)
{
	uart_init();
	uart_receiveByte();	
    
    while(1)
    {
		if(bit_is_set(PINB, 0)){
			
			if (prevPulse == 0)
			{
				prevPulse = 1;
				StartTimer1();
			}
		}
		if(bit_is_clear(PINB, 0))
			if (prevPulse == 1)
			{
				prevPulse = 0;
				StopTimer1();
			}
		}
}
