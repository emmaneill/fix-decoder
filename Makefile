CC=gcc -g
LDFLAGS += -I/usr/include/libxml2 -lxml2 
test: test.o parseXml.o hashTable.o

clean:
	rm -f test test.o parseXml.o hashTable.o

