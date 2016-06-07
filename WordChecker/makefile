CC=gcc
CFLAGS=-c -O2 -std=c99 -Wall -pedantic -Wmissing-declarations
SOURCES=*.c
OBJECTS=$(SOURCES:.c=.o)
OUTPUT=wordchecker.a

all: $(OUTPUT)
	
$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm $(OBJECTS) 
	rm $(OUTPUT)

