#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include "clksys_driver.h"
#include "qtouch/touch_api.h"

#define MEASUREMENT_PERIOD_MS       25u

/* flag set by timer ISR when it's time to measure touch */
static volatile uint8_t senseFlag = 0u;

/* current time, set by timer ISR */
static volatile uint16_t time_ms = 0u;
static volatile char red_led = 0x00;
void init_qt_globals (void);
void init_system (void);
void init_timer_isr (void);

int
main (void)
{ 

  init_system ();
  while (1) {
  }
}

void
init_qt_globals ()
{
  qt_config_data.qt_di = DEF_QT_DI;
  qt_config_data.qt_neg_drift_rate = DEF_QT_NEG_DRIFT_RATE;
  qt_config_data.qt_pos_drift_rate = DEF_QT_POS_DRIFT_RATE;
  qt_config_data.qt_max_on_duration = DEF_QT_MAX_ON_DURATION;
  qt_config_data.qt_drift_hold_time = DEF_QT_DRIFT_HOLD_TIME;
  qt_config_data.qt_recal_threshold = DEF_QT_RECAL_THRESHOLD;
}


void
init_system (void)
{
  /* Set up clocksystem to run at 8MHz using internal 32MHz Osc */
  CLKSYS_Enable (OSC_RC32MEN_bm);
  CLKSYS_Prescalers_Config (CLK_PSADIV_1_gc, CLK_PSBCDIV_4_1_gc);
  do {
  }
  while (CLKSYS_IsReady (OSC_RC32MRDY_bm) == 0);
  CLKSYS_Main_ClockSource_Select (CLK_SCLKSEL_RC32M_gc);

  init_qt_globals ();
  qt_enable_key (CHANNEL_0, AKS_GROUP_1, 10u, HYST_6_25);
  qt_enable_key (CHANNEL_1, AKS_GROUP_1, 10u, HYST_6_25);
  /* enable sensor 2: a slider on channels 2..4 */
  qt_enable_rotor (CHANNEL_5, CHANNEL_7, AKS_GROUP_1, 16u, HYST_6_25,RES_8_BIT, 0u);

  qt_init_sensing ();
  init_qt_globals ();

  init_timer_isr ();
  sei ();
  
  /* PORTF is connected to the Buttons, so se configure them as input with pullups */
  PORTF.DIRCLR = 0xFF;
  PORTCFG.MPCMASK = 0xFF;
  PORTF.PIN0CTRL = (PORTF.PIN0CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc;

  /* PORTE is connected to the LEDS, so we set this port as output so we can force it high/low */
  PORTE.DIRSET = 0xFF;
  PORTE.OUTSET = 0xFF;
  init_pwm_isr();

}

void
init_timer_isr (void)
{
  /*Set timer period A to ~25ms */
  TCC0.PER = TICKS_PER_MS * MEASUREMENT_PERIOD_MS;
  /*select clock source */
  TCC0.CTRLA = 4;
  /*Set Comparre A interrupt to low level */
  TCC0.INTCTRLB = 1;
  /*enable low lever interrupts in power manager interrupt control */
  PMIC.CTRL |= 1;
}

/** 
  Timer0 ISR Routine
  Triggered every 25ms. Each time it is triggered, it reads the values of the QTouch board sensors.
  It then calls a function to control the RGB LED based on the inputs.
**/

ISR (TCC0_CCA_vect) 
{
  char val = 0x00;
  time_ms += MEASUREMENT_PERIOD_MS;
  qt_measure_sensors (time_ms);  
  char s_states = qt_measure_data.qt_touch_status.sensor_states[0];
  if (s_states & (1 << 0)) {  /// Top button
    ///@TODO: Code to change color change state to prev color
  }
  if (s_states & (1 << 1)) {  /// Bottom button
    ///@TODO: Code to change color change state to next color
  }
  if (s_states & (1 << 2)) { /// Center rotor
    uint16_t sliderval = qt_measure_data.qt_touch_status.rotor_slider_values[0];
    sliderval = 0xff - sliderval;
    val = sliderval;
  }
  //PORTE.OUT = ~val;
}



void
init_pwm_isr (void)
{
  /*Set timer period A to ~25ms */
  TCD0.PER = 0x23;
  /*select clock source */
  TCD0.CTRLA = 4;
  /*Set Comparre A interrupt to low level */
  TCD0.INTCTRLA = TC_OVFINTLVL_LO_gc;
  TCD0.INTCTRLB = 0xff;//TC_CCDINTLVL_LO_gc;
  /*enable low lever interrupts in power manager interrupt control */
  PMIC.CTRL |= 1;
}

/** 
  Timer0 ISR Routine
  Triggered every 25ms. Each time it is triggered, it reads the values of the QTouch board sensors.
  It then calls a function to control the RGB LED based on the inputs.
**/

ISR (TCD0_CCA_vect) 
{
  PORTE.OUT = 0x00; 
  _delay_ms(1000);
}

ISR (TCD0_OVF_vect) 
{
  PORTE.OUT = 0xFF;
}