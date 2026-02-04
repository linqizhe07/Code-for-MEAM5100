//Linqi Zhe
#include "MEAM_general.h"

//Constants
#define TIMER_TOP 312   // F_CPU/256/200Hz - 1

/**
 * pulse_led: A subroutine to create smooth interpolated intensity changes.
 * @param start_intensity: Starting brightness percentage (0-100)
 * @param end_intensity: Ending brightness percentage (0-100)
 * @param duration_ms: Total time for the transition in milliseconds
 */

void pulse_led(int start_p, int end_p, int duration_ms) {
    int steps = 50;                             // Number of discrete steps for the transition
    int delay_per_step = duration_ms / steps;   // Duration to hold each intensity level
    
    for (int i = 0; i <= steps; i++) {
        /* Linear Interpolation Logic: Current = Start + (Progress * Total_Delta) 

        int16_t current_p = start_p + ((int16_t)i * (end_p - start_p) / steps);
        
        /* Translate 0-100% intensity to 0-312 Timer1 counts.
           OCR1A determines the "Off-point" (Integration Upper Limit) within the PWM cycle. */
        OCR1A = (uint16_t)(current_p * 312 / 100); 
        
        /* Note:Standard _delay_ms() requires a compile-time constant. 
           subloop allows for variable timing by accumulating 1ms increments. 
        */
        for (int d = 0; d < delay_per_step; d++) {
            _delay_ms(1); //I wrote _delay_ms(delay_per_step) only to find error.
        }
    }
}

int main(void) {
    _clockdivide(0);
    set(DDRB, 5); // Output on PB5

    // Timer 1: Mode 14 (Fast PWM), Prescaler 256, Non-inverting
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);
    ICR1 = TIMER_TOP;

    while (1) {
        // --- Heartbeat Cycle Starts ---

        // 0 to 100 in 0.1s, then 100 to 0 in 0.4s (Total 0.5s)
        pulse_led(0, 100, 100);
        pulse_led(100, 0, 400);

        // 0 to 50 in 0.1s, then 50 to 0 in 0.4s (Total 0.5s)
        pulse_led(0, 50, 100);
        pulse_led(50, 0, 400);

        // REST: 0 brightness for 2.0s
        OCR1A = 0;
        _delay_ms(2000); 

        // Cycle repeats every 3 seconds as per lab requirements
    }
}
