#include "MEAM_general.h"
#include <avr/io.h>
#include <stdint.h>

static inline void adc_select(uint8_t ch)
{
    // Reference = AVcc, right-adjust result
    clear(ADMUX, REFS1);
    set(ADMUX, REFS0);
    clear(ADMUX, ADLAR);

    // Select ADC channel (handle MUX5 for ch 8..13)
    if (ch >= 8) { set(ADCSRB, MUX5); ch -= 8; }
    else         { clear(ADCSRB, MUX5); }

    ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
}

void adc_init(uint8_t ch)
{
    // Enable ADC, prescaler = 128 (16MHz -> 125kHz ADC clock)
    set(ADCSRA, ADEN);
    set(ADCSRA, ADPS2);
    set(ADCSRA, ADPS1);
    set(ADCSRA, ADPS0);

    // Single-conversion, polling
    clear(ADCSRA, ADATE);
    clear(ADCSRA, ADIE);

    adc_select(ch);
}

uint16_t adc_read(void)
{
    // Start conversion, wait, return 10-bit result
    set(ADCSRA, ADSC);
    while (bit_is_set(ADCSRA, ADSC)) { ; }

    uint8_t low  = ADCL;   // read low first
    uint8_t high = ADCH;
    return ((uint16_t)high << 8) | low;
}

uint16_t adc_read_channel(uint8_t ch)
{
    adc_select(ch);
    return adc_read();
}