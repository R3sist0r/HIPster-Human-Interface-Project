#include "define.h"
#define ubrr 51

void uart_init(_Bool isr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter */
	UCSR0B |= _BV(RXEN0)|_BV(TXEN0);
    if(isr) 
        UCSR0B |= _BV(RXCIE0);   //Set recieve interrupt, but only if the function is passed true as a param
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);
	return;
}

void uart_tx( unsigned int data )
{
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
	return;
}

void uart_tx_str (char str[])
{
  for (int i = 0; i < (strlen (str)); i++)
    {
      while (!(UCSR0A & (1 << UDRE0)));
	  uart_tx (str[i]);
    }
}

#if ISR_STATE == 1
# warning "Interrupt being included"
    ISR(USART_RX_vect) {
        pwm_change(UDR0);
    }
#endif
