#include "define.h"

#define ADC_BRIGHTNESS    0
#define ADC_RED           _BV(MUX0)
#define ADC_GREEN         _BV(MUX1)
#define ADC_BLUE          _BV(MUX1)|_BV(MUX0)
#define ADMUX_CLR() {ADMUX &= !_BV(MUX1)&!_BV(MUX0);}
enum {BRIGHTNESS, RED, GREEN, BLUE}adc_next;
enum {new, old}adc_state;

void adc_init(void) {
  ADCSRA |= _BV(ADEN)|_BV(ADATE)|_BV(ADIE);   //Enable ADC with free running mode, and enable the interrupt service routine
  ADMUX |= _BV(REFS0)|_BV(ADLAR);             //Select AVCC as a voltage reference, and enable left adjust to give 8bit resolution
  adc_next = BRIGHTNESS;
  adc_state = new;
}

void adc_startConversion(void) {
  ADCSRA |= _BV(ADSC);    //start conversion
}

ISR(ADC_vect) {
  char mode = (ADMUX&0x0F);
  switch(mode) {
    case BRIGHTNESS:
        ADMUX++;
		pwm_setBrightness(ADCH);
    break;
    case RED:
      ADMUX++;
      pwm_sendRed(ADCH);
    break;
    case GREEN:
      ADMUX++;
      pwm_sendGreen(ADCH);
    break;
    case BLUE:
      ADMUX &= 0xF8;
      pwm_sendBlue(ADCH);
    break;
  }
}
