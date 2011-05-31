#include "define.h"
#define ADC_BRIGHTNESS    0
#define ADC_RED           _BV(MUX0)
#define ADC_GREEN         _BV(MUX1)
#define ADC_BLUE          _BV(MUX1)|_BV(MUX0)

void adc_init(void) {
  ADCSRA |= _BV(ADEN)|_BV(ADATE)|_BV(ADIE);   //Enable ADC with free running mode, and enable the interrupt service routine
  ADMUX |= _BV(REFS0)|_BV(ADLAR);             //Select AVCC as a voltage reference, and enable left adjust to give 8bit resolution
}

ISR(ADC_VECT) {
  
}
