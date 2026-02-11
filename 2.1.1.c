/* Name: 2.1.1.c
 * Author: <Linqi Zhe>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

#include <avr/io.h>

int main(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    set_led(ON);			// turn on the on board LED
    _delay_ms(1000);		// wait 1000 ms when at 16 MHz

    DDRC &= ~(1 << PC7);   // PC7 input
    PORTC &= ~(1 << PC7);  // no pull-up (external pull-down)
    DDRB |= (1 << PB6);   // Output

    while(1)
    {
        if (PINC & (1 << PC7)) {
          PORTB |= (1 << PB6);   // LED ON
      } 
        else {
          PORTB &= ~(1 << PB6);  // LED OFF
      }
    }
    return 0;  
}

