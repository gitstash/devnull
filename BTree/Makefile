CC=g++
TARGET+=btree.o
CFLAGS+=-g -ansi -Wall -std=c++11  
#LDFLAGS+=-lpthread 
SOURCES+=btree.cpp 
AR+=ar rcs
LIB_TARGET+=libbtree.a
MAIN_SOURCE+=test-tree.cpp
LIB+=btree

all: 
	${CC} ${CFLAGS} -c -o ${TARGET} ${SOURCES} 
lib: 
	${AR} ${LIB_TARGET} ${TARGET}
bin:
	${CC} ${CFLAGS} -L/home/manisha/BloombergPrep/BTree  -l ${LIB} -o ${btree} ${MAIN_SOURCE}

clean:
	rm -rf *.o ${TARGET}






