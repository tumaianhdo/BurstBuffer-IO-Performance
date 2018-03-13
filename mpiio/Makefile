CC := mpicc
CXX := mpicxx
FC := mpif90

COMPILE_FLAGS := 
LINK_FLAGS :=

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, %.o, ${SOURCES})

all : writer reader

writer: writer.o mpiio.o utils.o data.o
	${CC} -o $@ $^ ${LINK_FLAGS}

reader: reader.o mpiio.o utils.o data.o
	${CC} -o $@ $^ ${LINK_FLAGS}

%.o : %.c
	${CC} ${COMPILE_FLAGS} -I. -c $< -o $@

clean:
	rm -f writer reader *.o
