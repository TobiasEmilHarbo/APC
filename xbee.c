#define F_CPU 8000000UL 

// define baudrate for serial communication
#define BAUD 9600                               
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

#include <util/delay.h>
#include <avr/io.h>


//Method declarations
void receiveData(void); 

void uart_transmitByte (unsigned char);
void readSignalOnPin(void);

typedef unsigned char BYTE;

int readSignalStrength(BYTE byte);
unsigned int signalStrength; // Variable to hold ADC result
unsigned char uart_receiveByte (void);
unsigned char data, i;
unsigned char message[10];

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
	DDRB= 0b11111111;									
	PORTB=0b00000000;									
	
	uart_init();
	 
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);       // ADEN: Set to turn on ADC , by default it is turned off
        												// ADPS2: ADPS2 and ADPS0 set to make division factor 32
	
	while(1){
		//readSignalOnPin();

		receiveData();
		
		signalStrength = readSignalStrength(0b00000011);

		sprintf(message, "%d", signalStrength); //Convert signalStrength to array of chars
		
		for(i=0;message[i];i++)
		{		
			uart_transmitByte(message[i]);
		}
		
		_delay_ms(1000);
	}
}

void receiveData (void)
{

	data = uart_receiveByte();

	if(data=='1'){
		PORTB=0b00000010;						//turn on LED if received character is '1'
	}
	else 
	{	
		PORTB=0b00000000;	
	}
}

int readSignalStrength(BYTE byte)
{
        ADMUX = byte;// ADC input channel set to pin specified by 'byte'
        ADCSRA |= (1<<ADSC); // Start conversion
        while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
 
        return ADCW;
}

