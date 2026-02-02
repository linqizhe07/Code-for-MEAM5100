#include "MEAM_general.h"


int main(void)
{
    // Disable clock prescaler to run at full system frequency (typically 16MHz)
    _clockdivide(0);          
    
    // Configure PB5 as output
    set(DDRB, 5);

    /* Configure Waveform Generation Mode (WGM)
       Mode 14: WGM13:11 = 1, WGM10 = 0 
    */
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    /*
       Configure Compare Output Mode for Channel A
       COM1A1 = 1: Clear at OCR1A, set at rollover
    */
    TCCR1A |= (1 << COM1A1);

    /* Set Clock Select (Prescaler) to 256
       Timer frequency = F_CPU / 256
    */
    TCCR1B |= (1 << CS12);

    /*
       Frequency Calculation (TOP value):
       Formula: f_pwm = f_clk / (N * (1 + TOP))
       For 18Hz: 18 = 16,000,000 / (256 * (1 + ICR1))
       Result: ICR1 = 3471
    */
    ICR1 = 3471;

    /*
       Duty Cycle Configuration (OCR1A):
       To achieve 50% duty cycle: OCR1A = ICR1 / 2
       Corrected value: 3471 / 2 = 1735
    */
    OCR1A = 1735; 
    while(1);
}
