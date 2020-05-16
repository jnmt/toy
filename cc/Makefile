T0 = cc-test
OBJ_T0 = main.o operation.o util.o \
         transaction_base.o \
         transaction_ss2pl.o \
         transaction_occ.o

# For Mac OS
# CC = clang++
# CFLAGS = -g -std=c++17 -c -Wall -O2 -I/usr/local/include -D_DEBUG
# LDFLAGS = -L/usr/local/lib \
#           -lboost_system -lboost_thread-mt -lboost_program_options

# For Linux
CC = g++
CFLAGS = -g -std=c++17 -c -Wall -O2
LDFLAGS = -lboost_system -lboost_thread -lboost_program_options

all: $(T0)

$(T0): $(OBJ_T0)
	$(CC) -o $@ $^ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f *~ *.o $(T0)
