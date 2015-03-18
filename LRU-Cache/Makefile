CC=g++
TARGET+=lru
CFLAGS+=-g -ansi -Wall  -std=c++0x 
#LDFLAGS+=-lpthread 
SOURCES+=ll.cpp hash.cpp lru.cpp
#AR+=ar rcs
#MAIN_SOURCE+=test-tree.cpp
#LIB+=btree

all: 
	${CC} ${CFLAGS} -o ${TARGET} ${SOURCES} 
#lib: 
#	${AR} ${LIB_TARGET} ${TARGET}
#bin:
#	${CC} ${CFLAGS} -L/home/manisha/BloombergPrep/BTree  -l ${LIB} -o ${btree} ${MAIN_SOURCE}

clean:
	rm -rf *.o ${TARGET}






