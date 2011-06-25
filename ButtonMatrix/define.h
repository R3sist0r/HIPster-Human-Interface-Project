#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "../Generic/uart.h"
#include "../Generic/pwm.h"
#define _MATRIX_

void uart_tx( unsigned int data );
void uart_tx_str (char str[]);
void uart_init(_Bool isr);

int main(void);
void init_all(void);

void kb_init(void);
void kb_decode(unsigned char sc);
