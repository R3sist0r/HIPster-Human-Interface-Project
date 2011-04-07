##
## makefile
##
## Used for compiling rf24g example code
## Target: ATMEGA88 :: AVR-GCC
##
## Written by Tony Myatt - Quantum Torque, 2007
##

# Makefile configuration
MCU = atxmega128a1
FORMAT = ihex
TARGET = main
SRC = $(TARGET).c clksys_driver.c

ASRC = qtouch/qt_asm_xmega.S

# Compiler/Linker and other programs
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

# Compiler/Linker flags
# Genering flags for both assembler and C files

GFLAGS = -D_QTOUCH_ -D_ROTOR_SLIDER_ -DQT_NUM_CHANNELS=8 -DSNSK1=A -DSNS1=D -DQT_DELAY_CYCLES=1 

CFLAGS += -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums $(GFLAGS)
CFLAGS += -Wall -Wstrict-prototypes -Wa,-adhlns=$(<:.c=.lst) -std=gnu99
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref qtouch/libavrxmega7g1-8qt-k-2rs.a
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs $(GFLAGS)


# Programming support using avrdude 
AVRDUDE_FLAGS =  -cavrispmkII -p x128a1 -Pusb -F
AVRDUDE_WRITE_FLASH = -Uflash:w:$(TARGET).hex:i

# Define directories
DIRAVR = c:/winavr
DIRAVRBIN = $(DIRAVR)/bin
DIRAVRUTILS = $(DIRAVR)/utils/bin
DIRINC = .
DIRLIB = $(DIRAVR)/avr/lib

# Define all object files
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 

# Define all listing files
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)  
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Targets for building
all: $(TARGET).elf $(TARGET).hex size

program: $(TARGET).hex
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
	
clean:
	rm -f $(TARGET).elf $(TARGET).hex $(OBJ:.o=.lst) $(OBJ)

size:
	avr-size -A $(TARGET).elf

# Rules for output files
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

%.o : %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@
	
%.o : %.S
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@