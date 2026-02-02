#include "MEAM_general.h"

// ---- Pin selection ----
#define LED_BIT 5   // PB5

int main(void)
{
    _clockdivide(0);          // 16 MHz
    set(DDRB, LED_BIT);       // PB5 as output
 m,
    // ---- Duty cycle control (0 to 100) ----
    uint8_t duty_percent = 50;

    // ---- Software PWM period ----
    // Total PWM period in milliseconds (scope will show ~1/PERIOD frequency)
    const uint16_t PERIOD_MS = 20;   // 20 ms => 50 Hz

    while (1) {

        // Normal case: 1~99% duty cycle
        // Compute ON/OFF time within one PWM period
        uint16_t on_ms  = (uint16_t)((PERIOD_MS * duty_percent) / 100);
        uint16_t off_ms = PERIOD_MS - on_ms;

        // Output PWM waveform
        set(PORTB, LED_BIT);
        _delay_ms(on_ms);

        clear(PORTB, LED_BIT);
        _delay_ms(off_ms);
    }
}