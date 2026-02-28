#include "MEAM_general.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define TICKS_22HZ_MIN   10000
#define TICKS_22HZ_MAX   13000
#define TICKS_770HZ_MIN  280
#define TICKS_770HZ_MAX  380
/*
 * Timer3 timing notes:
 * - CPU clock: 16 MHz
 * - Timer3 prescaler: 64  => timer tick = 16MHz/64 = 250 kHz => 4 us per tick
 *
 * Frequency -> period -> ticks:
 * - 22 Hz  -> ~45.45 ms  -> ~11364 ticks
 * - 770 Hz -> ~1.30 ms   -> ~325 ticks
 *
 * The ranges below allow some tolerance for jitter.
 */

#define GLITCH_TICKS_MIN 150     // reject intervals < 150 ticks (~0.6 ms) as bounce/noise

static volatile uint16_t g_pulseTicks = 0;
static volatile uint8_t  g_newPulse   = 0;

/*
 * External Interrupt INT0 (PD0):
 * Each rising edge indicates a new period measurement.
 * Read Timer3, optionally reject very short intervals as noise, then reset Timer3.
 */
ISR(INT0_vect)
{
    uint16_t dt = TCNT3;

    // Ignore spurious edges that occur too quickly (likely noise/bounce)
    if (dt < GLITCH_TICKS_MIN) {
        return;
    }

    g_pulseTicks = dt;
    TCNT3 = 0;
    g_newPulse = 1;
}

static inline void leds_off(void)
{
    PORTB &= ~((1 << PB5) | (1 << PB6));
}

static inline void led_red_on_green_off(void)
{
    PORTB |=  (1 << PB5);
    PORTB &= ~(1 << PB6);
}

static inline void led_green_on_red_off(void)
{
    PORTB |=  (1 << PB6);
    PORTB &= ~(1 << PB5);
}

static void init_systems(void)
{
    // PB5 (D9) and PB6 (D10) as outputs (LEDs)
    DDRB |= (1 << DDB5) | (1 << DDB6);

    // PD0 (INT0 / pin 3) as input
    DDRD &= ~(1 << DDD0);

    // Timer3 prescaler = 64 (CS31=1, CS30=1)
    TCCR3B |= (1 << CS31) | (1 << CS30);

    // INT0 on rising edge (ISC01=1, ISC00=1)
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);

    sei(); // enable global interrupts
}

int main(void)
{
    init_systems();

    // Simple “no-signal” timeout counter; if too long without pulses, turn LEDs off.
    uint32_t noPulseCounter = 0;

    while (1) {
        if (g_newPulse) {
            g_newPulse = 0;
            noPulseCounter = 0;

            uint16_t t = g_pulseTicks;

            if (t > TICKS_770HZ_MIN && t < TICKS_770HZ_MAX) {
                // ~770 Hz detected -> green LED (D10)
                led_green_on_red_off();
            } else if (t > TICKS_22HZ_MIN && t < TICKS_22HZ_MAX) {
                // ~22 Hz detected -> red LED (D9)
                led_red_on_green_off();
            } else {
                // Other frequencies (e.g., noise) -> both off
                leds_off();
            }
        } else {
            // If no pulses for a while, shut everything off (~order of 0.5 s depending on loop speed)
            noPulseCounter++;
            if (noPulseCounter > 100000UL) {
                leds_off();
                noPulseCounter = 0;
            }
        }
    }
}
