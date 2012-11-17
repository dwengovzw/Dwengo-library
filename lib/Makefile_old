SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TARGET=dwengo.lib

MCC_C=$(MCC_BIN)/mcc18
MCC_L=$(MCC_BIN)/mplib

CFLAGS=-ls --no-extended -ml -p=18f4550

all: check $(TARGET)

check:
	@if [ ! -x ${MCC_C} ]; then echo ${MCC_C} not found, please make sure MCC_BIN is set correctly; false; fi
	@if [ ! -x ${MCC_L} ]; then echo ${MCC_L} not found, please make sure MCC_BIN is set correctly; false; fi
	@if [ ! -e ${MCC_INCLUDE}/p18f4550.h ]; then echo ${MCC_INCLUDE}/p18f4550.h not found, please make sure MCC_INCLUDE is set correctly; false; fi

%.o: %.c
	${MCC_C} $(CFLAGS) $<

$(TARGET): $(OBJECTS)
	rm -f $(TARGET)
	${MCC_L} -q -c $(TARGET)
	${MCC_L} -q -r $(TARGET) $(OBJECTS)

clean:
	rm -f *.o $(TARGET)
