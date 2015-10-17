#define F_CPU 4000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//Method declarations
void uart_transmitByte (unsigned char);
void uart_receiveByte (void);
/*
typedef unsigned char BYTE;
int readSignalStrength(BYTE byte);
int signalStrength; // Variable to hold ADC result
*/

unsigned char data, i, receivedData, message[10];

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
/*
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
}*/

int main( void)
{
	uart_init();
	
	_delay_ms(500);
	
	DDRC  = 0b11111111; // Set Port C as Output
	PORTC = 0b00000000;

    
    while(1)
    {

    	uart_receiveByte();	

    	PORT_OFF(PORTC,5);
    	PORT_OFF(PORTC, 4);
    	PORT_OFF(PORTC, 3);
    	
    	if(receivedData == '0')
    	{
    		PORT_ON(PORTC,5);
    		PORT_OFF(PORTC, 4);
    		PORT_OFF(PORTC, 3);
    	}
    	else if(receivedData == '1')
    	{
    		PORT_ON(PORTC,5);
    		PORT_ON(PORTC, 4);
    		PORT_OFF(PORTC, 3);
    	}
    	
    	else if(receivedData == '2')
    	{
    		PORT_ON(PORTC,5);
    		PORT_ON(PORTC, 4);
    		PORT_ON(PORTC, 3);
    	}
    	else
    	{
    		PORT_OFF(PORTC,5);
    		PORT_OFF(PORTC, 4);
    		PORT_OFF(PORTC, 3);
    	}

		/*if(bit_is_set(PINB, 0)){
			
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
			}*/
	}
}

/*
int readSignalStrength(BYTE byte)
{
        ADMUX = byte;// ADC input channel set to pin specified by 'byte'
        ADCSRA |= (1<<ADSC); // Start conversion
        while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
 
        return ADCW;
}*/


