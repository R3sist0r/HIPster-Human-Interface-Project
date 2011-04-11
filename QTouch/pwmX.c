/*!
*	\file pwm.c
*
*	\author (c)2011 Reuben Posthuma
*
*	\brief This file handles low level control of the pwm modules of the AVR, as well as 
		implementing functions to send commands to the RGB LED
	
	
*/
#include "../Generic/led.h"
#include <avr/io.h>
#include <avr/interrupt.h>
/*! \def INCREASE
	\brief This variable controls the step size of each brightness up/down recursion.
*/
#define INCREASE  20

#define PWM_PORT PORTE

#define PWM_TC  TCE0
#define PWM_PER PWM_TC.PER
#define PWM_CTRLA PWM_TC.CTRLA
#define PWM_CTRLB PWM_TC.CTRLB

#define GREEN_PWM PWM_TC.CCA
#define BLUE_PWM  PWM_TC.CCB
#define RED_PWM   PWM_TC.CCC

#define GREEN_PWM_EN TC0_CCAEN_bm
#define BLUE_PWM_EN  TC0_CCBEN_bm
#define RED_PWM_EN   TC0_CCCEN_bm

void pwm_init(void);
void pwm_setRed(char red);
void pwm_setGreen(char green);
void pwm_setBlue(char blue);
void pwm_setColour(char red, char green, char blue);
///Variable used for string conversions during debugging
char conv_str[10];

///Variable to hold the brightness of the red LED
char redR = 0;

///Variable to hold the brightness of the green LED
char greenR = 0;		

///Variable to hold the brightness of the blue LED
char blueR = 0;		

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

  PWM_PORT.DIRSET = 0xFF;
  PWM_PORT.OUTSET = 0xFF;
  
  PWM_PER = 0xFF;   //Set the period to 0xFF, effectively creating an 8bit PWM
  PWM_CTRLB = TC_WGMODE_SS_gc;  //Set the timer up for PWM generation mode
  PWM_CTRLA = TC_CLKSEL_DIV64_gc;   //Set the prescaler to clock/64

	//************ Timer/Counter E0A PWM setup (Green) ****************//
  PWM_CTRLB |= GREEN_PWM_EN;     //Enable the green LED
  GREEN_PWM = 0xFF;              //Set the green LED's brightness (inverse)
	//************ Timer/Counter E0B PWM setup (Blue) ****************//	
  PWM_CTRLB |= BLUE_PWM_EN;     //Enable the blue LED
  BLUE_PWM = 0xFF;              //Set the blue LED's brightness (inverse)
	//************ Timer/Counter E0C PWM setup (Red) ****************//	
  PWM_CTRLB |= RED_PWM_EN;        //Enable the red LED
  RED_PWM = 0xFF;                 //Set the red LED's brightness (inverse)
  
  PMIC.CTRL |= 1;
  sei();
}

void pwm_setRed(char red) {
  RED_PWM = redR = ~red;
}


void pwm_setGreen(char green) {
  GREEN_PWM = greenR = ~green;
}


void pwm_setBlue(char blue) {
  BLUE_PWM = blueR = ~blue;
}

void pwm_setColour(char red, char green, char blue) {
  RED_PWM = redR = ~red;
  GREEN_PWM = greenR = ~green;
  BLUE_PWM = blueR = ~blue;
}