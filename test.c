#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>
 

int main()
{
	//Configure PB0 as output, rest as input
    DDRB    = 0b00000000;
    PORTB   = 0b00000000;

    //DDRB    = 0b00000000;
    //PORTB   = 0b00000000;

    while(1)
    {
        PORTB ^= _BV(PB0);
        _delay_ms(500);
    }

}