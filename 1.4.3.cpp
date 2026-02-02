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
