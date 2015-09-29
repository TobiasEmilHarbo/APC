#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>
 

int main()
{
	//Configure ports as input or output 
    DDRB    |= _BV(DDB0);

    while(1)
    {
        PORTB ^= _BV(PB0);
        _delay_ms(5000);
    }

}