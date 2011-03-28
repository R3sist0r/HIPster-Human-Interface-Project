#include "define.h"

void init_all() {
	uart_init();
	kb_init();
	pwm_init();
	uart_tx_str("Booted up correctly!\r\n");
	sei();		//Global interrupt enable
	
}

int main() {
	init_all();
	while(1) {
		continue;
	}
}