T0 = forloop
T1 = volcano
T2 = data-centric
T3 = data-centric-cb
T4 = data-centric-cb-opt

CC = g++
CFLAGS = -g -c -Wall -O2
LDFLAGS =

OBJ_T0 = forloop.o
OBJ_T1 = volcano.o
OBJ_T2 = data-centric.o
OBJ_T3 = data-centric-cb.o
OBJ_T4 = data-centric-cb-opt.o

all: $(T0) $(T1) $(T2) $(T3) $(T4)

$(T0): $(OBJ_T0)
	$(CC) $(LDFLAGS) -o $@ $^

$(T1): $(OBJ_T1)
	$(CC) $(LDFLAGS) -o $@ $^

$(T2): $(OBJ_T2)
	$(CC) $(LDFLAGS) -o $@ $^

$(T3): $(OBJ_T3)
	$(CC) $(LDFLAGS) -o $@ $^

$(T4): $(OBJ_T4)
	$(CC) $(LDFLAGS) -o $@ $^

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o *.exe *.stackdump $(T0) $(T1) $(T2) $(T3) $(T4)
