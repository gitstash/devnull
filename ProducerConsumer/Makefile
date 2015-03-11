CC=g++
TARGET+=event.o queue.o
CFLAGS+=-g -ansi -Wall -std=c++11  
#LDFLAGS+=-lpthread 
SOURCES+= queue.cpp event.cpp
AR+=ar rcs
LIB_TARGET+=libevent.a
MAIN_SOURCE+=test_event.cpp
LIB+=event
BIN+=event

all: 
	${CC} ${CFLAGS} -c ${SOURCES} 
lib: 
	${AR} ${LIB_TARGET} ${TARGET}
bin:
	${CC} ${CFLAGS} -L/home/manisha/BloombergPrep/ProducerConsumer  -l ${LIB} -o ${BIN} ${MAIN_SOURCE}

clean:
	rm -rf *.o ${TARGET}






