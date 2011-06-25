#define RED_ROW     0
#define GREEN_ROW   1
#define BLUE_ROW    2
#define BRIGHT_ROW  3

enum matrixState_t{NONE, UP, OFF, DOWN};

void matrix_init(void);
void matrix_scan(void);
void matrix_action(void);

#define MATRIX_COLUMN_PORT_DIR      DDRC
#define MATRIX_COLUMN_PORT          PORTC
#define MATRIX_COLUMN_PORT2_DIR		DDRD
#define MATRIX_COLUMN_PORT2			PORTD
#define MATRIX_ROW_PORT_DIR         DDRC
#define MATRIX_ROW_PORT             PORTC
#define MATRIX_ROW_PORT_INPUT       PINC

#define MAT_ROW0    PC0
#define MAT_ROW1    PC1
#define MAT_ROW2    PC2
#define MAT_ROW3    PC3

#define MAT_COL0    PC4    
#define MAT_COL1    PC5    
#define MAT_COL2    PD2

#define NUM_COLS    3
#define NUM_ROWS    4

#define INC_MAX(var, max) {if(var==max) var=0; else var++;}
