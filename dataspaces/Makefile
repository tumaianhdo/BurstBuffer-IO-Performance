CC := mpicc
CXX := mpicxx
FC := mpif90

override DSPACES_DIR := ${SCRATCH}/software/install/dataspaces
override DSPACES_INC := $(shell ${DSPACES_DIR}/bin/dspaces_config -c)
override DSPACES_LIB := $(shell ${DSPACES_DIR}/bin/dspaces_config -l)

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, %.o, ${SOURCES})

all : writer reader 

writer: common.o put.o writer.o
	${CC} -o $@ $^ ${DSPACES_LIB}

reader: common.o get.o reader.o
	${CC} -o $@ $^ ${DSPACES_LIB}

%.o : %.c
	${CC} ${DSPACES_INC} -c $< -o $@

clean:
	rm -f reader writer *.o
