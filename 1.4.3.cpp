#include "MEAM_general.h"

// Hardware Constants
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
        /* Linear Interpolation Logic: Current = Start + (Progress * Total_Delta)  */
        int32_t current_p = start_p + ((int32_t)i * (end_p - start_p) / steps);
        
        /* Translate 0-100% intensity to 0-312 Timer1 counts.
           OCR1A determines the "Off-point" (Integration Upper Limit) within the PWM cycle. */
        OCR1A = (uint16_t)(current_p * 312 / 100); 
        
        /* 
           Nested loop allows for variable timing by accumulating 1ms increments. */
        for (int d = 0; d < delay_per_step; d++) {
            _delay_ms(1); 
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


    for ( int beat = 0; beat < 20; beat++) {
        // Linear decay: 100, 95, 90 ... down to 5
        int current_max = 100 - beat * 5;

        // Large Intensity: Peak at current_max
        pulse_led(0, current_max, 100);
        pulse_led(current_max, 0, 400);
        
        // Small Intensity: Peak at 50% of current_max
        int current_smaller = current_max / 2;
        pulse_led(0, current_smaller, 100);
        pulse_led(current_smaller, 0, 400);

        // REST: 0 brightness for 2.0s
        OCR1A = 0;
        _delay_ms(2000); 

    }
}
