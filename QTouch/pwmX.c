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

#define PWM_PORT PORTF

#define PWM_TC  TCF0
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
void pwm_setRed(uint8_t red);
void pwm_setGreen(uint8_t green);
void pwm_setBlue(uint8_t blue);
void pwm_setColour(uint8_t red, uint8_t green, uint8_t blue);
void pwm_setBrightness(uint8_t brightness);
///Variable used for string conversions during debugging
uint8_t conv_str[10];

///Variable to hold the brightness of the red LED
uint8_t redR = 0;

///Variable to hold the brightness of the green LED
uint8_t greenR = 0;		

///Variable to hold the brightness of the blue LED
uint8_t blueR = 0;		

uint8_t brightFactor = 1;
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

  PWM_PORT.DIRSET = 0x07;
  PWM_PORT.OUTSET = 0x00;
  
  PWM_PER = 0xFF;   //Set the period to 0xFF, effectively creating an 8bit PWM
  PWM_CTRLB = TC_WGMODE_SS_gc;  //Set the timer up for PWM generation mode
  PWM_CTRLA = TC_CLKSEL_DIV64_gc;   //Set the prescaler to clock/64

	//************ Timer/Counter E0A PWM setup (Green) ****************//
  PWM_CTRLB |= GREEN_PWM_EN;     //Enable the green LED
  GREEN_PWM = 0x00;              //Set the green LED's brightness (inverse)
	//************ Timer/Counter E0B PWM setup (Blue) ****************//	
  PWM_CTRLB |= BLUE_PWM_EN;     //Enable the blue LED
  BLUE_PWM = 0x00;              //Set the blue LED's brightness (inverse)
	//************ Timer/Counter E0C PWM setup (Red) ****************//	
  PWM_CTRLB |= RED_PWM_EN;        //Enable the red LED
  RED_PWM = 0x00;                 //Set the red LED's brightness (inverse)
  
  PMIC.CTRL |= 1;
  sei();
}

void pwm_setRed(uint8_t red) {
  redR = red;
  RED_PWM = red/brightFactor;
}


void pwm_setGreen(uint8_t green) {
  greenR = green;
  GREEN_PWM = green/brightFactor;
}


void pwm_setBlue(uint8_t blue) {
  blueR = blue;
  BLUE_PWM = blue/brightFactor;
}

void pwm_setColour(uint8_t red, uint8_t green, uint8_t blue) {
  RED_PWM = redR = red;
  GREEN_PWM = greenR = green;
  BLUE_PWM = blueR = blue;
}

void pwm_setBrightness(uint8_t brightness) {
  brightFactor = 255/(brightness+1);
  RED_PWM = redR/brightFactor;
  GREEN_PWM = greenR/brightFactor;
  BLUE_PWM = blueR/brightFactor; // blueR = 
}