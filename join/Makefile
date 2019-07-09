T1 = volcano

CC = g++
CFLAGS = -g -c -Wall -O2
LDFLAGS =

OBJ_T1 = volcano.o

all: $(T1)

$(T1): $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^

$(T2): $(OBJ_T2)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1)
