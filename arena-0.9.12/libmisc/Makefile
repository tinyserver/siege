SOURCES = misc_error.c misc_state.c
OBJECTS = misc_error.o misc_state.o
LIB = libmisc.a

CC = gcc
CFLAGS = -g -O2 -fno-strict-aliasing -Wall

all: $(LIB)

$(LIB): $(OBJECTS)
	ar -crs $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS)
	rm -f $(LIB)

distclean: clean
	rm -f Makefile

realclean: clean

depend: $(SOURCES)
	$(CC) $(CFLAGS) -MM $(SOURCES) >> Makefile

# automatically generated dependencies follow
