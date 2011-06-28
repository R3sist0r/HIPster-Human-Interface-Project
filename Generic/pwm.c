/*!
*	\file pwm.c
*
*	\author (c)2011 Reuben Posthuma
*
*	\brief This file handles low level control of the pwm modules of the AVR, as well as 
		implementing functions to send commands to the RGB LED
	
	
*/

#include "define.h"
#include "led.h"
#ifdef _MATRIX_
	#include "matrix.h"
#endif

#include "pwm.h"
/*! \def INCREASE
	\brief This define controls the step size of each brightness up/down recursion.
*/

#define SET_GREEN_PWM(_GREEN) {OCR0A = ~_GREEN; greenR = _GREEN;}
#define SET_BLUE_PWM(_BLUE) {OCR2A = ~_BLUE; blueR = _BLUE;}
#define SET_RED_PWM(_RED) {OCR1A = (16*_RED); redR = _RED;}

  
#define SET_GREEN_PWMH(_GREEN) {OCR0A = 255-_GREEN; }
#define SET_BLUE_PWMH(_BLUE) {OCR2A = 255-_BLUE; }
#define SET_RED_PWMH(_RED) {OCR1A = (16*_RED); }
#define top(_OUT, _VALUE, _ADD) {if((_VALUE+_ADD)>255) _OUT=255; else _OUT=_VALUE+_ADD;}
#define bottom(_OUT,_VALUE, _SUBTRACT) {if((_VALUE-_SUBTRACT)<0) _OUT=255; else _OUT=_VALUE-_SUBTRACT;}
///Variable used for string conversions during debugging
char conv_str[10];

///Variable to hold the brightness of the red LED
char redR = 0;

///Variable to hold the brightness of the green LED (0xFF, as that register is inverse)
char greenR = 0x00;		

///Variable to hold the brightness of the blue LED (0xFF, as that register is inverse)
char blueR = 0x00;		

uint8_t brightFactor = 256;

/*! 	
	\brief A function which initialises the pwm module of the AVR

	pwm_init() sets up the required AVR registers, including setting outputs, and setting PWM modes. 
	It then turns the RGB LED off.
	
	Initialisation sequence:
	- Set all pins as outputs
	- Set up each timer for PWM mode
		-# Non-inverting PWM mode
		-# Fast PWM mode
		-# Select the clock source as un-prescaled
	
*/
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

/*! 	
	\brief A function which effectively creates any colour on the RGB LED, sent in RGB notation
	\param red 		The red value (0-255) to send to the LED
	\param green 	The red value (0-255) to send to the LED
	\param blue 	The red value (0-255) to send to the LED
	
*/
void pwm_sendColor(int red, int green, int blue) {
	SET_RED_PWM(led_correct[red]);
	SET_GREEN_PWM(led_correct[green]);
	SET_BLUE_PWM(led_correct[blue]);
}


void pwm_sendRed(char red) {
  SET_RED_PWM(led_correct[red]);
}

void pwm_sendGreen(char green) {
  SET_GREEN_PWM(led_correct[green]);
}

void pwm_sendBlue(char blue) {
  SET_BLUE_PWM(led_correct[blue]);
}

/*! 	
	\brief Set the RED LED to maximum brightness
*/

void pwm_maxRed(void) {
		OCR1A = 1020;
		redR = 255;
}

/*! 	
	\brief Increment the red LED by the value set in INCREASE	
		
	Since the red LED is connected to a 10bit pwm, however, it multiplies the result by 4 to provide the correct value
*/

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

/*! 	
	\brief Decrement the red LED by the value set in INCREASE	
		
	Since the red LED is connected to a 10bit pwm, however, it multiplies the result by 4 to provide the correct value
*/
void pwm_decRed(void) {
	if(redR <= INCREASE) 
		OCR1A = redR = 0;
	else {
		OCR1A = 4*led_correct[redR - INCREASE];
		redR = redR-INCREASE;
	}
}

/*! 	
	\brief Set the RED LED to minimum brightness
*/

void pwm_minRed(void) {
    OCR1A = 0;
    redR = 0;
}

/*! 	
	\brief Set the green LED to its maximum brightness
*/
void pwm_maxGreen(void) {
    OCR0A = 0;
    greenR = 255;
}

/*! 	
	\brief Increment the green LED by the value set in INCREASE	
*/
void pwm_incGreen(void) {
	if(greenR + INCREASE >= 255) {
		OCR0A = 0;
		greenR = 255;
	}
	else {
		OCR0A = led_correct[255-(greenR + INCREASE)];
		greenR = greenR+INCREASE;
	}
}

/*! 	
	\brief Decrement the green LED by the value set in INCREASE	
*/
void pwm_decGreen(void) {
	if(greenR - INCREASE <= 0) {
		OCR0A = 255;
		greenR = 0;
	}
	else {
		OCR0A = led_correct[255-(greenR - INCREASE)];
		greenR = greenR-INCREASE;
	}
}

/*! 	
	\brief Set the green LED to minimum brightness
*/
void pwm_minGreen(void) {
    OCR0A = 255;
    greenR = 0;
}

/*! 	
	\brief Set the Blue LED to its maximum brightness
*/
void pwm_maxBlue(void) {
    OCR2A = 0;
    blueR = 255;
}

/*! 	
	\brief Increment the blue LED by the value set in INCREASE	
*/
void pwm_incBlue(void) {
	if(blueR + INCREASE >= 255) {
		OCR2A = 0;
		blueR = 255;
	}
	else {
		OCR2A = led_correct[255-(blueR + INCREASE)];
		blueR = blueR+INCREASE;
	}
}

/*! 	
	\brief Decrement the blue LED by the value set in INCREASE
*/
void pwm_decBlue(void) {
	if(blueR - INCREASE <= 0) {
		OCR2A = 255;
		blueR = 0;
	}
	else {
		OCR2A = led_correct[255-(blueR - INCREASE)];
		blueR = blueR-INCREASE;
	}
}


/*! 	
	\brief Set the Blue LED to its minimum brightness
*/
void pwm_minBlue(void) {
    OCR2A = 255;
    blueR = 0;
}

void pwm_setBrightness(uint8_t brightness) {
  brightFactor = brightness;
  SET_RED_PWMH(led_correct[(uint8_t)(redR*brightFactor/256)]);
  SET_GREEN_PWMH(led_correct[(uint8_t)(greenR*brightFactor/256)]);
  SET_BLUE_PWMH(led_correct[(uint8_t)(blueR*brightFactor/256)]);
}

/*! 	
	\brief Act on the keyboard character recieved from the PS/2 keyboard or UART port
	This function uses a switch statement to determine what action to take, based on the input from the keyboard
	
	\param kbchar 	The processed character recieved from the keyboard

*/
void pwm_change(char kbchar) {
	switch(kbchar) {
        case 'R':
			pwm_maxRed();
		break;
		case 'r':
			pwm_incRed();
		break;
		case 'e':
			pwm_decRed();
		break;		
        case 'E':
			pwm_minRed();
		break;
		case 'G':
			pwm_maxGreen();
		break;		
        case 'g':
			pwm_incGreen();
		break;
		case 'f':
			pwm_decGreen();
		break;		
        case 'F':
			pwm_minGreen();
		break;
		case 'B':
			pwm_maxBlue();
		break;		
        case 'b':
			pwm_incBlue();
		break;
		case 'v':
			pwm_decBlue();
		break;
		case 'V':
			pwm_minBlue();
		break;		
        case 'o':
			pwm_sendColor(0,0,0);
		break;
        case 'w':
			pwm_sendColor(255,255,255);
		break;
		default:
		break;
	}
}
#ifdef _MATRIX_

	void pwm_select(uint8_t color, enum matrixState_t action) {							
		static uint8_t redTimeout, greenTimeout, blueTimeout, brightTimeout;
		switch(color) {
			case RED_ROW:
				if(redTimeout == 0) {
					redTimeout = 10;
					switch(action) {
						case UP:
							pwm_incRed();
						break;
						case DOWN:
							pwm_decRed();
						break;
						case OFF:
							pwm_minRed();
						break;
						case NONE: break;
					}
				}
				redTimeout--;
			break;
			case GREEN_ROW:
				if(greenTimeout == 0) {
					greenTimeout = 10;
					switch(action) {
						case UP:
							pwm_incGreen();
						break;
						case DOWN:
							pwm_decGreen();
						break;
						case OFF:
							pwm_minGreen();
						break;
						case NONE: break;
					}
				}
				greenTimeout--;
			break;
			case BLUE_ROW:
				if(blueTimeout == 0) {
					blueTimeout = 10;
					switch(action) {
						case UP:
							pwm_incBlue();
						break;
						case DOWN:
							pwm_decBlue();
						break;
						case OFF:
							pwm_minBlue();
						break;
						case NONE: break;
					}
				}
				blueTimeout--;
			break;
			case BRIGHT_ROW:
				if(brightTimeout == 0) {
					brightTimeout = 10;
					switch(action) {
						case UP:
							if(brightFactor>200){
								pwm_setBrightness(255);
							}
							else {
								uint8_t temp = (brightFactor+10);
								pwm_setBrightness(temp);
							}
						break;
						case DOWN:
							if((brightFactor-INCREASE)<INCREASE) {
								pwm_setBrightness(0);
							}
							else  {
								uint8_t temp = (brightFactor-INCREASE);
								pwm_setBrightness(temp);
							}
						break;
						case OFF:
							pwm_setBrightness(0);
						break;
						case NONE: break;
					}
				}
				brightTimeout--;
			break;
		}
	}

#endif
