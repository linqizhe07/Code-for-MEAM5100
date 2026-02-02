#include "MEAM_general.h"

#define LED_BIT 5   // Pin selection 

int main(void)
{
    _clockdivide(0);          // Default, 16 MHz
    set(DDRB, LED_BIT);       // PB5 as output

    TCCR1A = 0;            //Normal Mode
    TCCR1B = 0;

    //set prescaler as 256 (CS12=1, CS11=0, CS10=0)
    TCCR1B = (1 << CS12);

    // (16M/256) / 18 = 3472;  65536 - 3472 = 62064
    TCNT1 = 62064;

    while (1) {
        //Check if the timer has reached its overflow value
        if (TIFR1 & (1 << TOV1)){

            PORTB ^= (1<< LED_BIT); //Switch off PORTB

            TCNT1 = 62064;          //Reset the value

            TIFR1 |= (1 << TOV1);   //Reset the overflow flag
                                    // Note: In AVR, writing 1 to a flag bit clears it
        }
    }
}
