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

void pwm_init(void);
void pwm_setRed(char red);
void pwm_setGreen(char green);
void pwm_setBlue(char blue);
void pwm_setColour(char red, char green, char blue);
void pwm_setBrightness(char brightness);
volatile enum colours {red, green, blue} colour_state;
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
  colour_state = red;
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
  qt_enable_slider (CHANNEL_2, CHANNEL_4, AKS_GROUP_1, 16u, HYST_6_25,RES_8_BIT, 0u);
  qt_enable_rotor (CHANNEL_5, CHANNEL_7, AKS_GROUP_1, 16u, HYST_6_25, RES_8_BIT, 0u);

  qt_init_sensing ();
  init_qt_globals ();
  init_timer_isr ();
  pwm_init();
  sei ();
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
  static char disable;
  time_ms += MEASUREMENT_PERIOD_MS;
  qt_measure_sensors (time_ms);  
  char s_states = qt_measure_data.qt_touch_status.sensor_states[0];
  if (s_states & (1 << 0)) {  /// Top button
    if(disable>0)
      disable--;
    else {
     if(colour_state != blue)
        colour_state++;
      else  
        colour_state = red;
      
      disable = 10;
    }
  }
  if (s_states & (1 << 1)) {  /// Bottom button
    if(disable>0)
      disable--;
    else {
      if(colour_state != red)
        colour_state--;
      else 
        colour_state = blue;
      disable = 10;
    }
  }
  if (s_states & (1 << 2)) { /// Left slider
    uint8_t sliderval = qt_measure_data.qt_touch_status.rotor_slider_values[0];
    switch(colour_state) {
      case red:
        pwm_setRed(sliderval);
      break;
      case green:
        pwm_setGreen(sliderval);
      break;
      case blue:
        pwm_setBlue(sliderval);
      break;
      default:
        pwm_setColour(255,255,255);
      break;
    }
  }
  
  if (s_states & (1 << 3)) { /// Center rotor
    uint8_t rotorval = qt_measure_data.qt_touch_status.rotor_slider_values[1];
    uint8_t brightness = ~rotorval;
    pwm_setBrightness(brightness);
  }
}