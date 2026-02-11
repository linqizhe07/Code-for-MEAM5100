#include "MEAM_general.h"
#include "m_usb.h"
#include <avr/io.h>

/**
 * ATmega32U4 Input Capture (Timer 3) on IPC3
 */

int main(void) {
    _clockdivide(0);         
    m_usb_init();            
    
    while(!m_usb_isconnected()); 

    /* 2. PC7 (ICP3) Pin Configuration */
    DDRC &= ~(1 << PC7);     // Configure PC7 as an input
    PORTC |= (1 << PC7);     // Enable internal pull-up resistor

    /* 3. Timer 3 Configuration */
    /* Set Clock Prescaler to 1024 
       Timer Frequency: 16MHz / 1024 = 15.625 kHz*/
    TCCR3B |= (1 << CS32) | (1 << CS30); 
    
    /* Input Capture Edge Select (ICES3):
       Setting ICES3 to 0 configures the hardware to trigger on a FALLING edge. */
    TCCR3B &= ~(1 << ICES3); 


    uint16_t current_time = 0;
    uint16_t last_time = 0;
    uint16_t delta = 0;
    int is_first_press = 1;


    while(1) {
        /* 4. Monitor Input Capture Flag (ICF3)
           The hardware sets this bit when a falling edge is detected on PC7. */
        if (TIFR3 & (1 << ICF3)) {
            
            /* ICR3 captures the TCNT3 value at the instant the edge hit. */
            current_time = ICR3; 

            if (!is_first_press) {
                /* Calculate the time difference (Delta). */
                delta = current_time - last_time;

                /* Transmit the delta value*/
                m_usb_tx_string("Interval: ");
                m_usb_tx_int(delta); 
                m_usb_tx_string(" ticks\n");
            } else {
                /* Establish the first reference point without printing a delta. */
                is_first_press = 0;
                m_usb_tx_string("Measurement started...\n");
            }

            /* Record the current time to be used as 'last_time' in the next iteration. */
            last_time = current_time;

            /* Reset the flag: Writing a '1' to ICF3 clears it in the hardware register. */
            TIFR3 |= (1 << ICF3);
        }
    }
    
    return 0;
}