CC=gcc
CFLAGS=-c -O2 -std=c99 -Wall -pedantic -Wmissing-declarations
SOURCEDIR=WordChecker
SOURCES=$(wildcard $(SOURCEDIR)/*.c)
OBJECTS=$(SOURCES:$(SOURCEDIR)/%.c=%.o)
OUTPUT=wordchecker.a

all: $(OUTPUT)
	
$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm $(OBJECTS) 
	rm $(OUTPUT)

