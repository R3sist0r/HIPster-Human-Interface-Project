#include "define.h"
#include "scancodes.h"

#define PORT_CLK PORTD		//The port of the clock line, for setting to input
#define PORT_DATA PORTB		//The port of the data line, for setting to input 
#define PIN_CLK PIND		//The read register of the clock line for reading it
#define PIN_DATA PINB		//The read register of the data line for reading it
#define P_CLK PD2			//The actual pin name of the clock line
#define P_DATA PB6			//The actual pin name of the data line

_Bool edge = 0;
char bitcount = 11;
char conv_str[10];

void kb_init(void) {
	PORT_CLK &= !_BV(P_CLK);		//Ensure that the clock pin is an input
	PORT_DATA &= !_BV(P_DATA);		//Ensure that the data pin is an input
	EICRA |= _BV(ISC01); 		//INT0 interrupt on the falling edge
	EIMSK |= _BV(INT0);			//Enable INT0 interrupt
	MCUCR |= _BV(IVCE);
	MCUCR |= _BV(IVSEL);
	uart_tx_str("Keyboard hardware initialised\r\n");
	
}


void kb_decode(unsigned char sc)
{
    static unsigned char is_up=0, shift = 0, mode = 0;
    unsigned char i;

    if (!is_up)                // Last data received was the up-key identifier
    {
        switch (sc)
        {
          case 0xF0 :        // The up-key identifier
            is_up = 1;
            break;

          case 0x12 :        // Left SHIFT
            shift = 1;
            break;

          case 0x59 :        // Right SHIFT
            shift = 1;
            break;

          case 0x05 :        // F1
            if(mode == 0)
                mode = 1;    // Enter scan code mode
            if(mode == 2)
                mode = 3;    // Leave scan code mode
            break;

          default:
            if(mode == 0 || mode == 3)        // If ASCII mode
            {
                if(!shift)                    // If shift not pressed,
                {                            // do a table look-up
                    for(i = 0; unshifted[i][0]!=sc && unshifted[i][0]; i++);
                    if (unshifted[i][0] == sc) {
                        pwm_change(unshifted[i][1]);
                    }
                } else {                    // If shift pressed
                    for(i = 0; shifted[i][0]!=sc && shifted[i][0]; i++);
                    if (shifted[i][0] == sc) {
                        pwm_change(shifted[i][1]);
                    }
                }
            } else{                            // Scan code mode
                uart_tx_str(itoa(sc, conv_str, 16));            // Print scan code
                uart_tx(' ');
                uart_tx(' ');
            }
            break;
        }
    } else {
        is_up = 0;                            // Two 0xF0 in a row not allowed
        switch (sc)
        {
          case 0x12 :                        // Left SHIFT
            shift = 0;
            break;
            
          case 0x59 :                        // Right SHIFT
            shift = 0;
            break;

          case 0x05 :                        // F1
            if(mode == 1)
                mode = 2;
            if(mode == 3)
                mode = 0;
            break;
          case 0x06 :
            break;
        } 
    }    
} 


ISR(INT0_vect) {
	static unsigned char data;				// Holds the received scan code

    if (!edge)								// Routine entered at falling edge
    {
        if(bitcount < 11 && bitcount > 2)	// Bit 3 to 10 is data. Parity bit,
        {									// start and stop bits are ignored.
            data = (data >> 1);
            if(PIN_DATA & _BV(P_DATA))			//If the keyboard has put a 1 on the line
                data = data | 0x80;			// Store a '1'
        }

        EICRA |= _BV(ISC00);				// Set interrupt on rising edge
        edge = 1;
        
    } else {								// Routine entered at rising edge

        EICRA &= !_BV(ISC00);				// Set interrupt on falling edge
        edge = 0;

        if(--bitcount == 0)					// All bits received
        {
			kb_decode(data);
			bitcount = 11;
        }
    }
}