#define F_CPU 8000000UL 

#include <util/delay.h>
#include <avr/io.h>

typedef unsigned char BYTE;

//Method declarations
void receiveData(void); 
unsigned char uart_receiveByte (void);

void transmitData(unsigned char);
void uart_transmitByte (unsigned char);

int readSignalStrength(BYTE byte);
void readSignalOnPin(void);

// define baudrate for serial communication
#define BAUD 9600                               // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)        // set baud rate value for UBRR
  
unsigned char data;

int signalStrength; // Variable to hold ADC result

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

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);       // ADEN: Set to turn on ADC , by default it is turned off
        												// ADPS2: ADPS2 and ADPS0 set to make division factor 32
	
	while(1){
		readSignalOnPin();
		receiveData();
		
		if (signalStrength < 1023){
			transmitData('0');
		}

		if (signalStrength = 1023){
			transmitData('1');
		}
		_delay_ms(1000);
	}
}

void receiveData (void)
{
	data = uart_receiveByte();

	if(data=='1'){
		PORTC=0b00100000;						//turn on LED if recieved character is '1'
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

int readSignalStrength(BYTE byte)
{
        ADMUX = byte;// ADC input channel set to pin specified by 'byte'
        ADCSRA |= (1<<ADSC); // Start conversion
        while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
 
        return ADCW;
}

void readSignalOnPin(void)
{      

    signalStrength = readSignalStrength(0b00000100); //PC0

}