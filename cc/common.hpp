#pragma once
#include <vector>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#define DEFAULT_NUM_THREADS 4
#define DEFAULT_NUM_OPERATIONS 10
#define DEFAULT_TABLE_SIZE 10000

#define OP_READ  0
#define OP_WRITE 1

#define TX_INPROGRESS 0
#define TX_ABORT 1
#define TX_COMMIT 2

#ifdef _DEBUG
#define DEBUG(x) do { std::cerr << x << " "; } while (0)
#define DEBUGV(x) do { std::cerr << #x << ": " << x << " "; } while (0)
#else
#define DEBUG(x)
#define DEBUGV(x)
#endif

class Record {
public:
  boost::upgrade_mutex m;
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

class Transaction {
private:
  boost::thread::id threadId;
  std::vector<Operation*> operations;
  std::vector<Operation*> readSet;
  std::vector<Operation*> writeSet;
  std::vector<boost::upgrade_mutex*> readLocks;
  std::vector<boost::upgrade_mutex*> writeLocks;
  Operation* searchReadSet(int key);
  Operation* searchWriteSet(int key);
  void eraseFromReadSet(Operation *op);
  void eraseFromWriteSet(Operation *op);
  void eraseFromReadLocks(boost::upgrade_mutex* m);
  void releaseLock();
  void debug(int op, int key, bool success);
  void debug(boost::format fmt);
public:
  int status;
  Transaction();
  ~Transaction();
  void begin(const boost::thread::id tid);
  void commit();
  void abort();
  void read(Operation *op);
  void write(Operation *op);
  void generateOperations(int generateOperations, int readRatio);
  std::vector<Operation*> getOperations();
  Record* getRecord(int key);
};

extern struct timeval cur_time(void);
extern void print_performance(int numTxns,
                              struct timeval begin, struct timeval end);

extern boost::mutex stdout_mutex;
