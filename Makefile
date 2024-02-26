CC:= avr-gcc
COMPILE_OPTIONS:= -Wall -Werror -pedantic
DEV ?= /dev/ttyACM0
SRC_DIR:= $(abspath ./fsw/src)
OUT_DIR:= $(abspath ./)/out
LIB_DIR:= $(abspath ./)/out/lib
INC_DIR:= $(abspath ./)/out/include
TARGET:= $(OUT_DIR)/atum
HEX:= $(patsubst %,%.hex,$(TARGET))
DEFS:=
INCS:= -I$(INC_DIR)
SRCS:= $(SRC_DIR)/atum.c
LIBS:= $(LIB_DIR)/libMaat.a

MCU := atmega328p

export OUT_DIR MCU COMPILE_OPTIONS

.PHONY: all libs hex clean install

all: hex $(TARGET)

hex: $(TARGET)
	avr-objcopy -O ihex $(TARGET) $(HEX)

$(TARGET): $(OUT_DIR) libs
	$(CC) -mmcu=$(MCU) $(COMPILE_OPTIONS) $(DEFS) $(INCS) -o $@ $(SRCS) $(LIBS)

$(OUT_DIR):
	mkdir $@

clean:
	rm -r $(OUT_DIR)

install: hex
	avrdude -c arduino -p m328p -P $(DEV) -U flash:w:$(HEX)

libs:
	$(MAKE) -C $(abspath ./maat)