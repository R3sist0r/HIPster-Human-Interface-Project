
#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h>
#include "clksys_driver.h"
#include "qtouch/touch_api.h"

/*
* The timing information for timer to fire periodically to measure touch.
*/
#define TICKS_PER_MS                1000u
#define MEASUREMENT_PERIOD_MS       25u

/* flag set by timer ISR when it's time to measure touch */
static volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
static volatile uint16_t current_time_ms_touch = 0u;

void init_qt_globals(void);
void init_system(void);

int main(void) {
	init_system();
	init_qt_globals();
	
	qt_enable_key( CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25 );
	qt_enable_key( CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25 );
    /* enable sensor 2: a slider on channels 2..4 */
    qt_enable_rotor( CHANNEL_5, CHANNEL_7, AKS_GROUP_1, 16u, HYST_6_25, RES_8_BIT, 0u  );
	
	qt_init_sensing();
	init_qt_globals();
	
	init_timer_isr();
	sei();
	while(1) {
	 if( time_to_measure_touch ){
            /* clear flag: it's time to measure touch */
            time_to_measure_touch = 0u;

            /* measure touch sensors */
            qt_measure_sensors( current_time_ms_touch );
			
			
			char s_states = qt_measure_data.qt_touch_status.sensor_states[0];
			char val = 0x00;
			if(s_states & (1<<0)){	
				val = 0x33;
			}
			if(s_states & (1<<1)){	
				val = 0xCC;
			}
			if(s_states & (1<<2)){
				uint16_t sliderval = qt_measure_data.qt_touch_status.
									   rotor_slider_values[0];
				sliderval = 0xff - sliderval;
				val = 1<<(sliderval/32);
			}
			PORTE.OUT = ~val;
			/*
			char key_states = qt_measure_data.qt_touch_status.sensor_states[0];
			
			//Turn all lights off
			PORTE.OUT = 0xFF;
			
			if(key_states & 1){
				//Turn Left Side lights on
				PORTE.OUT = 0xF0;
			}
			
			if(key_states & 2) {
				PORTE.OUT = 0x0F;
			}*/
        }
	}
}

void init_qt_globals() {
	qt_config_data.qt_di = DEF_QT_DI;
	qt_config_data.qt_neg_drift_rate = DEF_QT_NEG_DRIFT_RATE;
	qt_config_data.qt_pos_drift_rate = DEF_QT_POS_DRIFT_RATE;
	qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
	qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
	qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
}


void init_system( void )
{
    /* Set up clocksystem to run at 8MHz using internal 32MHz Osc */
   CLKSYS_Enable( OSC_RC32MEN_bm );
   CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_4_1_gc );
   do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
   CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );

   /* PORTF is connected to the Buttons, so se configure them as input with pullups */
   PORTF.DIRCLR = 0xFF;
   PORTCFG.MPCMASK = 0xFF;
   PORTF.PIN0CTRL = (PORTF.PIN0CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; 

	/* PORTE is connected to the LEDS, so we set this port as output so we can force it high/low */
	PORTE.DIRSET = 0xFF;
	PORTE.OUTSET = 0xFF;
	
}

void init_timer_isr( void )
{   
   /*Set timer period A to ~25ms*/
   TCC0.PER = TICKS_PER_MS * MEASUREMENT_PERIOD_MS;
   /*select clock source*/
   TCC0.CTRLA = 4;
   /*Set Comparre A interrupt to low level*/
   TCC0.INTCTRLB = 1;
   /*enable low lever interrupts in power manager interrupt control*/
   PMIC.CTRL |= 1;   
}

/*============================================================================
  Name : timer_isr
------------------------------------------------------------------------------
Purpose: timer 1 compare ISR
Input  : n/a
Output : n/a
Notes  :
============================================================================*/

ISR(TCC0_CCA_vect)
{
    /* set flag: it's time to measure touch */
    time_to_measure_touch = 1u;

    /* update the current time */
    current_time_ms_touch += MEASUREMENT_PERIOD_MS;
}