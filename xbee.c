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

void vibrate (char i)
{

	switch(i)
    {
    case '1' :
            //vibration
            PORT_ON(PORTB,1);
            _delay_ms(1000);
            PORT_OFF(PORTB,1);
            _delay_ms(1000);

            PORT_ON(PORTB,1);
            _delay_ms(1000);
            PORT_OFF(PORTB,1);
            _delay_ms(1000);

            PORT_ON(PORTB,1);
            _delay_ms(1000);
            PORT_OFF(PORTB,1);
            _delay_ms(1000);

            break;
    case '2' :
            //vibration
            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);

            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);

            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);

            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);

            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);

            PORT_ON(PORTB,1);
            _delay_ms(500);
            PORT_OFF(PORTB,1);
            _delay_ms(500);
            
            break;
    }
}

int main(void)
{
	uart_init();
	
	_delay_ms(500);
	
	DDRC  = 0b11111111; // Set Port C as Output
	PORTC = 0b00000000;

	DDRB  = 0b11111110; // Set Port C as Output
	PORTB = 0b00000001;
    
    while(1)
    {

    	uart_receiveByte();	
    	
    	if(receivedData == '0')
    	{
    		//do nothing
    	}
    	else if(receivedData == '1')
    	{
    		vibrate('1');
    	}
    	
    	else if(receivedData == '2')
    	{

    		vibrate('2');
    	}
	}
}


