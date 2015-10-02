#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void USART_Init(void)
{
	//Set baud rate
	UBRRL=(uint8_t)UBRRVAL;	//low byte

	UBRRH=(UBRRVAL>>8);	//high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size

	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
	(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB=(1<<RXEN)||(1<<RXCIE);
	//enable global interrupts
}
	
uint8_t USART_vReceiveByte(void)
{
	// Wait until a byte has been received
	while((UCSRA&(1<<RXC)) == 0);
		// Return received data
		return UDR;
}

ISR(USART_RXC_vect)
{
	UDR=0;//clear UART buffer

	char data;	
	data = USART_vReceiveByte();//receive data
	
	if(data=='1'){
		PORTC=0b00100000;//turn on LED if recieved character is 'a'
	}
	else 
	{	
		PORTC=0b0000000;	
	}
}

int main(void)
{

	DDRC=0xFF;//make PORTC output
	PORTC=0x00;//set all outputs to 0
	
	sei();//enable global interrupts
	USART_Init();
	
	while(1)
	{

	}

} 

