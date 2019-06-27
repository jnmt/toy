T1 = bs
T2 = hash

CC = g++
CFLAGS = -g -c -Wall -O2
LDFLAGS =

OBJ_T1 = bs.o
OBJ_T2 = hash.o

all: $(T1) $(T2)

$(T1): $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^

$(T2): $(OBJ_T2)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T1) $(T2)
