#include "MEAM_general.h"

int main(void)
{
    _clockdivide(0); // 16MHz

    // PC7: phototransistor circuit output node -> digital input
    DDRC  &= ~(1 << PC7);
    PORTC &= ~(1 << PC7);    //no pull-up

    // PB6: external LED output
    DDRB |= (1 << PB6);

    while (1)
    {
        // HIGH means dark (no light), LOW means light
        uint8_t dark = (PINC & (1 << PC7)) ? 1 : 0;

        if (dark)  PORTB |=  (1 << PB6);  // no light -> LED ON
        else       PORTB &= ~(1 << PB6);  // light    -> LED OFF

        _delay_ms(5);
    }
}