#include "MEAM_general.h"
#include "m_usb.h" 

/**
 * ATmega32U4 Switch & Timer Monitor
 * * Hardware Mapping:
 * - Input:  Switch on PC7
 * - Output: LED on PB6
 * - Clock:  16MHz (no prescaler)
 * - Timer:  Timer3 with 1024 prescaler
 */

int main(void)
{
    _clockdivide(0);         
    m_usb_init();            
    
    /* Timer3 Configuration: 
       Set CS32 and CS30 bits for 1024 prescaler. 
       Clock frequency = 16MHz / 1024 = 15,625 Hz */
    TCCR3B |= (1 << CS32) | (1 << CS30); 

    /* I/O Configuration */
    DDRC &= ~(1 << PC7);     // Clear PC7 bit: Set as Input
    DDRB |= (1 << PB6);      // Set PB6 bit: Set as Output

    uint8_t last_state = 0;

    /* Make sure USB connection is established */
    while(!m_usb_isconnected()); 

    while(1)
    {
        /* Read input state: Mask PINC to isolate PC7 and normalize to 0 or 1 */
        uint8_t current_state = (PINC & (1 << PC7)) ? 1 : 0;

        /* Trigger only when the switch state changes */
        if (current_state != last_state) {
            
            if (current_state) {
                PORTB |= (1 << PB6);             // LED ON
                m_usb_tx_string("Press! T3: "); 
            } else {
                PORTB &= ~(1 << PB6);            // LED OFF
                m_usb_tx_string("Release! T3: "); 
            }

            /* Log the current 16-bit Timer Counter value */
            m_usb_tx_int(TCNT3); 
            m_usb_tx_char('\n'); 

            /* Update state for the next iteration to detect the next transition */
            last_state = current_state; 
        }
    }
}