#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#define ISR_STATE 1

void uart_tx( unsigned int data );
void uart_tx_str (char str[]);
void uart_init(_Bool isr);

int main(void);
void init_all(void);

void kb_init(void);
void kb_decode(unsigned char sc);

void pwm_init(void);
void pwm_sendColor(int red, int green, int blue);
void pwm_incRed(void);
void pwm_decRed(void);
void pwm_incGreen(void);
void pwm_decGreen(void);
void pwm_incBlue(void);
void pwm_decBlue(void);
void pwm_change(char kbchar);