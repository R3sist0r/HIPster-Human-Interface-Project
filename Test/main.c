/*!
*	\file main.c
*
*	\author (c)2011 Reuben Posthuma
*
*	\brief HIPster: Human Interface Project .:. 
*	   Controlling an RGB LED with a PS/2 keyboard
*
*	HIPster is a study on various human interface devices.  
*	A number of different interface modules have been created, and they all are designed to perform one simple task:
*	controlling the color and intensity of an RGB LED.  This provides a consistent benchmark, with the aim of finding the most 
*	natural human-device interaction method.	      
*/

#include "define.h"

/*! 	
	\brief A function which initialises all required hardware 

	init_all() calls further functions to set up low-level hardware.  It also 
	prints a string on the UART port to show that the system has booted correctly
	
	Initialisation sequence:
	- Initialise the uart serial port
	- Initialise the keyboard interrupt hardware
	- Initialise the pwm module hardware for control of the RGB LED
	- Send a string through the serial port to show that the system has booted correctly
	- Enable global interrupts
	
*/

void init_all() {
	uart_init(ISR_STATE);		
	kb_init();			
	pwm_init();			
	uart_tx_str("Booted up correctly!\r\n");	
	sei();		
	
}

/*! 	
	\brief The main() function of the program

	Since the keyboard control of the RGB LED is entirely interrupt-driven, 
	once the init_all() routine has been called, the main routine is only an infinite loop.
*/
int main() {
	init_all();
  pwm_sendColor(125,30,90);
	while(1) {
		continue;
	}
}