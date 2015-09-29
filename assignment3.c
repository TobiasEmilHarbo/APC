#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>
 
//Declared variables and methods 
int playerOnePts;
int playerTwoPts;
int gameCount;
int startGame;

void playerOne(int i);
void playerTwo(int i);
void playerWins(int i);

int main()
{
	//Configure ports as input or output 
    DDRB    = 0b11111001;
    PORTB   = 0b00000110;

    DDRC    = 0b00011111;
    PORTC   = 0b11100000;

    DDRD    = 0b11111111;
    PORTD   = 0b00000000;

    //Initialize the declared variables
    playerOnePts = 0;
    playerTwoPts = 0;
    gameCount = 0; 
    startGame = 0;

    //Use the gamecount variable(number of games played) as input for the display method
    display(gameCount);
 
	while(1)
	{

		if (bit_is_clear(PINC, 5))
		{
			startGame = 1;
		}

		if (startGame == 1)
		{		
	        //Player 1 button - blau LEDs
	        //Check, if the button attached to PB1 is pressed
	        if (bit_is_clear(PINB, 1))
	        {
	            _delay_ms(5);
				//Check, if the button attached to PB1 is released
	            if (!bit_is_clear(PINB, 1))
	            {
	            	if (playerOnePts < 6)
	            	{
	            		//Count up the individual player-score (number of LEDs)
	            		playerOnePts++;
	            		playerOne(playerOnePts);
	            	}	
	            }
	        }

	        //Player 2 button - rotes LEDs
	        //Check, if the button attached to PB2 is pressed
	        if (bit_is_clear(PINB, 2))
	        {
	            _delay_ms(5);

	            //Check, if the button attached to PB2 is released
	            if (!bit_is_clear(PINB, 2))
	            {
	            	if (playerTwoPts < 6)
	            	{
	            		//Count up the individual player-score (number of LEDs)
	            		playerTwoPts++;
	            		playerTwo(playerTwoPts);
	            	}
	            }
	        }
    	}
    }
}
 
//Takes the current score of the player as input
//The switch takes care of ligthing the appropriate number of leds   
void playerOne(int i)
{

    PORTB &= 0b00000110;
    PORTD &= 0b01111111;

    switch(i)
    {
    case 1 :
            PORTB |= 0b00000001;
            break;
    case 2 :
            PORTB |= 0b00000001;
            PORTD |= 0b10000000;
            break;
    case 3 :
            PORTB |= 0b10000001;
            PORTD |= 0b10000000;
            break;
    case 4 :
            PORTB |= 0b11000001;
            PORTD |= 0b10000000;
            break;
    case 5:
            PORTB |= 0b11001001;
            PORTD |= 0b10000000;

            playerWins(1);

            break;
        }
}

//Same functionality as playerOne()
void playerTwo(int i)
{
    PORTC &= 0b11100000;

    switch(i)
    {
    case 1 :
            PORTC |= 0b00000001;
            break;
    case 2 :
            PORTC |= 0b00000011;
            break;
    case 3 :
            PORTC |= 0b00000111;
            break;
    case 4 :
            PORTC |= 0b00001111;
            break;
    case 5:
            PORTC |= 0b00011111;
            playerWins(2);
            break;
    }
}

//Blink the appropriate LED's, based on the input (1 or 2) from the playerOne() or playerTwo() methods
//When the winners LED's has blinked, reset variables and start next game 
void playerWins(int i)
{
	_delay_ms(200);

	PORTB &= 0b00000110;
    PORTD &= 0b01111111;
	PORTC &= 0b11100000;

	switch(i)
	{
		case 1:
        	_delay_ms(200);

			PORTB |= 0b11001001;
            PORTD |= 0b10000000;

            _delay_ms(100);

            PORTB &= 0b00000110;
        	PORTD &= 0b01111111;

        	_delay_ms(200);

        	PORTB |= 0b11001001;
            PORTD |= 0b10000000;

            _delay_ms(100);

        	PORTB &= 0b00000110;
        	PORTD &= 0b01111111;

        	_delay_ms(200);

        	PORTB |= 0b11001001;
            PORTD |= 0b10000000;

            _delay_ms(100);

        	PORTB &= 0b00000110;
        	PORTD &= 0b01111111;

			break;

		case 2:
        	_delay_ms(200);

			PORTC |= 0b00011111;

            _delay_ms(100);

            PORTC &= 0b11100000;

        	_delay_ms(200);

        	PORTC |= 0b00011111;

            _delay_ms(100);

        	PORTC &= 0b11100000;

        	_delay_ms(200);

        	PORTC |= 0b00011111;

            _delay_ms(100);

        	PORTC &= 0b11100000;

			break;	
	}

	gameCount++;

	if (gameCount > 9)
	{
		gameCount = 0;
		startGame = 0;
	}

	playerOnePts = 0;
    playerTwoPts = 0;
	
	display(gameCount);
}

//Light up the different combinations of LED's
//Each case corresponds to the actual number being displayed 
display(int i)
{
        PORTD &= 0b00000000;
 
        switch(i)
        {
            case 0:
                        PORTD |= 0b00111111;
                break;
            case 1 :
                        PORTD |= 0b00000110;
                break;
            case 2 :
                        PORTD |= 0b01011011;
                break;
            case 3 :
                        PORTD |= 0b01001111;
                break;
            case 4 :
                        PORTD |= 0b01100110;
                break;
            case 5 :
                        PORTD |= 0b01101101;
                break;
            case 6 :
                        PORTD |= 0b01111101;
                break;
            case 7 :
                        PORTD |= 0b00000111;
                break;
            case 8 :
                        PORTD |= 0b01111111;
                break;
            case 9 :
                        PORTD |= 0b01101111;
                break;
        }
}