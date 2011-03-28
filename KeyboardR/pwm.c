#include "define.h"
#include "led.h"
#define INCREASE 20		//defines the rate of change of LED brightness	

char conv_str[10];
char redR = 0;
char greenR = 0xFF;
char blueR = 0xFF;

void pwm_init(void) {
	DDRB |= _BV(PB1);
	DDRD |= _BV(PD3)|_BV(PD5);
	
	//************ Timer/Counter 0 PWM setup (GREEN) ****************//
	TCCR0A |= _BV(COM0B1); //Set output compare 2B to non-inverting fast PWM mode
	TCCR0A |= _BV(WGM01)|_BV(WGM00);	//Set the PWM module to fast PWM mode
	TCCR0B |= _BV(CS00)|_BV(WGM02);	//Select the clock source as un-prescaled, and set the PWM module to fast PWM mode
	
	//************ Timer/Counter 1 PWM setup (RED) ****************//	
	TCCR1A |= _BV(COM1A1); //Set output compare 2B to non-inverting fast PWM mode
	TCCR1A |= _BV(WGM11)|_BV(WGM10);	//Set the PWM module to fast PWM mode
	TCCR1B |= _BV(CS10)|_BV(WGM12);	//Select the clock source as un-prescaled, and set the PWM module to fast PWM mode

	//************ Timer/Counter 2 PWM setup (BLUE) ****************//	
	TCCR2A |= _BV(COM2B1); //Set output compare 2B to non-inverting fast PWM mode
	TCCR2A |= _BV(WGM21)|_BV(WGM20);	//Set the PWM module to fast PWM mode
	TCCR2B |= _BV(CS20)|_BV(WGM22);	//Select the clock source as un-prescaled, and set the PWM module to fast PWM mode	
	
	pwm_sendColor(0,0,0);
}

void pwm_sendColor(int red, int green, int blue) {
	OCR0A = ~green;
	
	int i = red;
	i = i*16;
	OCR1A = i;

	OCR2A = ~blue;
}

void pwm_incRed(void) {
	if(redR +INCREASE >= 255) {
		OCR1A = 1023;
		redR = 255;
	}
	else {
		OCR1A = 4*led_correct[redR + INCREASE];
		redR = redR+INCREASE;
	}
}

void pwm_decRed(void) {
	if(redR <= INCREASE) 
		OCR1A = redR = 0;
	else {
		OCR1A = 4*led_correct[redR - INCREASE];
		redR = redR-INCREASE;
	}
}

void pwm_incGreen(void) {
	if(greenR - INCREASE <= 0) 
		OCR0A = greenR = 0;
	else {
		OCR0A = led_correct[greenR - INCREASE];
		greenR = greenR-INCREASE;
	}
}

void pwm_decGreen(void) {
	if(greenR + INCREASE >= 255) 
		OCR0A = greenR = 255;
	else {
		OCR0A = led_correct[greenR + INCREASE];
		greenR = greenR+INCREASE;
	}
}

void pwm_incBlue(void) {
	if(blueR - INCREASE <= 0) 
		OCR2A = blueR = 0;
	else {
		OCR2A = led_correct[blueR - INCREASE];
		blueR = blueR-INCREASE;
	}
}

void pwm_decBlue(void) {
	if(blueR + INCREASE >= 255) 
		OCR2A = blueR = 255;
	else {
		OCR2A = led_correct[blueR + INCREASE];
		blueR = blueR+INCREASE;
	}
}

void pwm_change(char kbchar) {
	switch(kbchar) {
		case 'r':
			pwm_incRed();
			uart_tx_str(ltoa(OCR1A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		case 'e':
			pwm_decRed();
			uart_tx_str(ltoa(OCR1A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		case 'g':
			pwm_incGreen();
			uart_tx_str(ltoa(OCR0A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		case 'f':
			pwm_decGreen();
			uart_tx_str(ltoa(OCR0A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		case 'b':
			pwm_incBlue();
			uart_tx_str(ltoa(OCR2A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		case 'v':
			pwm_decBlue();
			uart_tx_str(ltoa(OCR2A, conv_str, 10));
			uart_tx_str("\r\n");
		break;
		default:
		break;
	}
	uart_tx(kbchar);
}