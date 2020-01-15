T0 = query-processor

CC = g++
CFLAGS = -g -c -Wall -O2
LDFLAGS =

OBJ_T0 = main.o
OBJ_T1 = operator.o

all: $(T0)

$(T0): $(OBJ_T0) $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T0)
