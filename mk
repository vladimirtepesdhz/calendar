
DEBUG = 0
CC = g++
TARGET = main.exe
ifeq ($(DEBUG),0)
DIR_BUILD = release
CFLAGS = -O2 -Wall
else
DIR_BUILD = debug
CFLAGS = -O2 -Wall -g
endif
DIR_OBJ = $(DIR_BUILD)/obj
DIR_BIN = $(DIR_BUILD)/bin
DIR_IND = $(DIR_BUILD)/ind
BIN_TARGET = ${DIR_BIN}/${TARGET}
SRC := 
OBJ := 
SRC += CDate.cpp
OBJ += ${DIR_OBJ}/CDate.o
SRC += Date.cpp
OBJ += ${DIR_OBJ}/Date.o

all:|createdir ${BIN_TARGET}

createdir:
	@if [ ! -d $(DIR_OBJ) ]; then\
		mkdir -p $(DIR_OBJ) ;\
	fi
	@if [ ! -d $(DIR_BIN) ]; then\
		mkdir -p $(DIR_BIN) ;\
	fi
	@if [ ! -d $(DIR_IND) ]; then\
		mkdir -p $(DIR_IND) ;\
	fi

${BIN_TARGET}:${OBJ}
	$(CC) ${OBJ} -o $@

${DIR_OBJ}/CDate.o: CDate.cpp CDate.h
	$(CC) $(CFLAGS) -c CDate.cpp -o $@
${DIR_OBJ}/Date.o: Date.cpp CDate.h
	$(CC) $(CFLAGS) -c Date.cpp -o $@

.PHONY:clean
clean:
	-del ${OBJ} ${BIN_TARGET}
	-rm -rf ${OBJ} ${BIN_TARGET}
