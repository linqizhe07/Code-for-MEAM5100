#include "MEAM_general.h"

// ---- Pin selection ----
#define LED_BIT 5   // PB5

int main(void)
{
    _clockdivide(0);          // 16 MHz
    set(DDRB, LED_BIT);       // PB5 as output

    const uint16_t PERIOD_MS = 20;   // 20 ms => 50 Hz

    while (1) {
        // On
        set(PORTB, LED_BIT);
        _delay_ms(200);

        // Off
        clear(PORTB, LED_BIT);
        _delay_ms(200);
    }
}
