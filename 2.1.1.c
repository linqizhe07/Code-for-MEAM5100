/* Name: 2.1.1.c
 * Author: <Linqi Zhe>
 */

#include "MEAM_general.h"  // includes the resources included in the MEAM_general.h file

#include <avr/io.h>

int main(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz

    DDRC &= ~(1 << PC7);   // PC7 as input
    PORTC &= ~(1 << PC7);  // No pull-up 
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



