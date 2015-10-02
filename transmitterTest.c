//https://amtek.wordpress.com/2010/11/07/configuring-and-using-xbee-wireless-modules/
#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUDRATE 9600
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void UART_transmit(unsigned char data);

int main(void)
{
unsigned char i,message[]="hey";
DDRD=0x00;
PORTD=0xFF;
	//Set baud rate
	UBRRL=(uint8_t)0;	//low byte

	UBRRH=0;	//high byte
	//Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size

	UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
	(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSRB=(1<<RXEN)|(1<<TXEN);
	//enable global interrupts
	
	while(1)
	{
		for(i=0;message[i];i++)
		{
			UART_transmit(message[i]);
		}
		_delay_ms(1000);
	} 
} 


void UART_transmit(unsigned char data)
{
	while(!(UCSRA & (1<<UDRE)));
		UDR=data;
}

/*void USART_Init(void)
{
	UCSRA=0;
	UCSRB=1<<TXEN; // transmitter enable
	UCSRC=1<<URSEL | 1<<UCSZ1 | 1<<UCSZ0; // 8 data bit, a stop, none parity
	UBRRL=(uint8_t)UBRRVAL;	//low byte

	UBRRH=(UBRRVAL>>8);	//high byte
}

uint8_t USART_vTransmitByte(unsigned char data)
{
	// Wait until a byte has been received
	while(!(UCSRA & (1<<UDRE))){
		
		UDR = data;
	}
}

int main(void)
{
	sei();//enable global interrupts
	USART_Init();
	
	while(1)
	{
		USART_vTransmitByte(61);
	}

} 
*/
