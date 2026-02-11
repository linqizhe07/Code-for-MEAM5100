#include "MEAM_general.h"
#include "m_usb.h"
#include <avr/io.h>

/* 2.1.4EC: pinMode(register, bit, mode) */
#define INPUT        1
#define INPUT_PULLUP 2
#define OUTPUT       3

/*
    pinMode(C, 7, INPUT_PULLUP);   // PC7 input with pullup
    pinMode(B, 6, OUTPUT);         // PB6 output
    pinMode(D, 2, INPUT);          // PD2 input, no pullup
*/
#define pinMode(PORT_LETTER, BIT, MODE)                           \
    do {                                                          \
        if ((MODE) == OUTPUT) {                                   \
            DDR##PORT_LETTER  |=  (1 << (BIT));                   \
        } else if ((MODE) == INPUT_PULLUP) {                      \
            DDR##PORT_LETTER  &= ~(1 << (BIT));                   \
            PORT##PORT_LETTER |=  (1 << (BIT));                   \
        } else { /* INPUT (floating) */                           \
            DDR##PORT_LETTER  &= ~(1 << (BIT));                   \
            PORT##PORT_LETTER &= ~(1 << (BIT));                   \
        }                                                         \
    } while (0)

/**
 * ATmega32U4 Input Capture (Timer 3) on ICP3 (PC7)
 * Prints tick intervals between falling edges on PC7.
 */
int main(void) {
    _clockdivide(0);
    m_usb_init();
    while(!m_usb_isconnected());

    /* Pin config using pinMode macro */
    pinMode(C, 7, INPUT_PULLUP);   // PC7 as input with pullup (ICP3 pin)

    /* Timer 3 prescaler = 1024 (16MHz/1024 = 15.625kHz) */
    TCCR3B |= (1 << CS32) | (1 << CS30);

    /* Capture on falling edge */
    TCCR3B &= ~(1 << ICES3);

    uint16_t current_time = 0;
    uint16_t last_time = 0;
    uint16_t delta = 0;
    uint8_t is_first_press = 1;

    while (1) {
        if (TIFR3 & (1 << ICF3)) {

            current_time = ICR3;

            if (!is_first_press) {
                delta = (uint16_t)(current_time - last_time);
                m_usb_tx_string("Interval: ");
                m_usb_tx_int(delta);
                m_usb_tx_string(" ticks\n");
            } else {
                is_first_press = 0;
                m_usb_tx_string("Measurement started...\n");
            }

            last_time = current_time;

            /* clear capture flag */
            TIFR3 |= (1 << ICF3);
        }
    }
    return 0;
}
