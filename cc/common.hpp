#pragma once
#include <vector>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#define DEFAULT_NUM_THREADS 4
#define DEFAULT_NUM_OPERATIONS 10
#define DEFAULT_TABLE_SIZE 10000

#define MODE_S2PL 0

#define OP_READ  0
#define OP_WRITE 1

#ifdef _DEBUG
#define DEBUG(x) do { std::cerr << x << " "; } while (0)
#define DEBUGV(x) do { std::cerr << #x << ": " << x << " "; } while (0)
#else
#define DEBUG(x)
#define DEBUGV(x)
#endif

class Record {
public:
  boost::upgrade_mutex m; // For SS2PL
  int attr;
};
extern std::vector<Record*> table;

class Operation {
private:
  int type;
public:
  int key;
  int value;
  Operation(int key);
  Operation(int key, int value);
  bool isRead();
};

extern struct timeval cur_time(void);
extern void print_performance(int numTxns,
                              struct timeval begin, struct timeval end);

extern boost::mutex stdout_mutex;
