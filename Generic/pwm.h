#define INCREASE 20		

void pwm_init(void);
void pwm_sendColor(int red, int green, int blue);
void pwm_incRed(void);
void pwm_decRed(void);
void pwm_incGreen(void);
void pwm_decGreen(void);
void pwm_incBlue(void);
void pwm_decBlue(void);
void pwm_change(char kbchar);

#ifdef _MATRIX_
	void pwm_select(uint8_t color, enum matrixState_t action);
#endif
