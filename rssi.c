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
void matTest (void);
void vibrate (char);

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
	TCCR1B |= (1<<CS12)|(0<<CS11)|(1<<CS10);
	//uart_transmitByte('s');
}

void StopTimer1(void)
{
	TCCR1B |= (0<<CS12)|(0<<CS11)|(0<<CS10);
	
	//sprintf(message, "%d", TCNT1); //Convert signalStrength to array of chars

	/*for(i=0;message[i];i++)
		{
			uart_transmitByte(message[i]);
		}
	*/
	
	if(TCNT1 > 10000){
		uart_transmitByte('c');		// If close, only transmit c
	}
	else{
		//uart_transmitByte('f');
		//_delay_ms(100);

		matTest(); 					// If far away, call matTest
	}
}

int main( void)
{
	uart_init();

	DDRC  = 0b11111111; // Set Port C as Output
	PORTC = 0b00000000;

	DDRB  = 0b11111110; // Set Port C as Output
	PORTB = 0b00000001;
    
    while(1)
    {

    	uart_receiveByte();	

		if(bit_is_set(PINB, 0)){
			
			if (prevPulse == 0)
			{
				prevPulse = 1;
				StartTimer1();
			}

		}
		
		else{
			if (prevPulse == 1)
			{
				prevPulse = 0;
				StopTimer1();
			}
		}
	}
}

void matTest(void)
{
    	// LEDs
    	PORT_OFF(PORTC,5);
    	PORT_OFF(PORTC, 4);
    	PORT_OFF(PORTC, 3);
    	
    	if(receivedData == '1')		// The value of the received data determines vibration mode
    	{
    		PORT_ON(PORTC,5);
    		PORT_ON(PORTC, 4);
    		PORT_OFF(PORTC, 3);

    		vibrate('1');
    	}
    	
    	else if(receivedData == '2')		// The value of the received data determines vibration mode
    	{
    		PORT_ON(PORTC,5);
    		PORT_ON(PORTC, 4);
    		PORT_ON(PORTC, 3);

    		vibrate('2');
    	}
}

void vibrate (char i)
{

	switch(i)
    {
    /*case '0' :
            //vibration
            PORT_ON(PORTB,1);
	
            break;
    */
    case '1' :
            //6 second vibration
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
            //6 second vibration
            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

                        //6 second vibration
            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);

            PORT_ON(PORTB,1);
            _delay_ms(250);
            PORT_OFF(PORTB,1);
            _delay_ms(250);
            
            break;
    }
}
