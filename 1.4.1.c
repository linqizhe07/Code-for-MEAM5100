//Author: Linqi Zhe
#include "MEAM_general.h"

// ---- Pin Definitions ----
#define LED_BIT 5   // PB5 (OC1A)

int main(void)
{
    // Disable clock prescaler to run at full system frequency (16MHz)
    _clockdivide(0);   
    
    // Configure PB5 as output
    DDRB |= (1 << LED_BIT);

    /* Configure Waveform Generation Mode (WGM)
       Mode 14: Up to ICR1, PWM Mode
       WGM13:11 = 1, WGM10 = 0 
    */
   /* Set Clock Select (Prescaler) to 256
       Timer frequency = F_CPU / 256
    */
       /*
       Configure Compare Output Mode for Channel A
       COM1A1 = 1: Clear at OCR1A, set at rollover
    */
    TCCR1A = (1 << WGM11) | (1 << COM1A1);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);

    /*
       Frequency Calculation (TOP value):
       Formula: f_pwm = f_clk / (N * (1 + TOP))
       For 200Hz: 200 = 16,000,000 / (256 * (1 + ICR1))
       Result: ICR1 = 312
    */
    ICR1 = 312;

    int i;

    while (1) {
        // 0.2s Fade in; total_steps = 100, each_step 2ms
        for (i = 0; i <= 100; i++){
            OCR1A = i * ICR1 / 100;
            _delay_ms(2);
        }

        // 0.7s Fade out; total_steps = 100, each_step 7ms
        for(i = 100; i > 0; i--){
            OCR1A = i * ICR1 / 100;
            _delay_ms(7);
        }
    }
}
