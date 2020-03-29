#pragma once
#include <vector>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "common.hpp"

#define TX_INPROGRESS 0
#define TX_ABORT 1
#define TX_COMMIT 2

class Transaction {
private:
  virtual void begin() = 0;
  virtual void commit() = 0;
  virtual void abort() = 0;
  virtual void read(Operation *op) = 0;
  virtual void write(Operation *op) = 0;
public:
  virtual void execute(const boost::thread::id tid) = 0;
  virtual void generateOperations(int numOperations, int readRatio) = 0;
};

class BaseTransaction : Transaction {
protected:
  int status;
  boost::thread::id threadId;
  std::vector<Operation*> operations;
  std::vector<Operation*> readSet;
  std::vector<Operation*> writeSet;
  void begin();
  void commit();
  void abort();
  void read(Operation *op);
  void write(Operation *op);
  std::vector<Operation*> getOperations();
  Operation* searchReadSet(int key);
  Operation* searchWriteSet(int key);
  void eraseFromReadSet(Operation *op);
  void eraseFromWriteSet(Operation *op);
  Record* getRecord(int key);
  void debug(boost::format fmt);
public:
  BaseTransaction();
  ~BaseTransaction();
  void execute(const boost::thread::id tid);
  void generateOperations(int numOperations, int readRatio);
};

class Ss2plTransaction : BaseTransaction {
private:
  std::vector<boost::upgrade_mutex*> readLocks;
  std::vector<boost::upgrade_mutex*> writeLocks;
  void begin();
  void commit();
  void abort();
  void read(Operation *op);
  void write(Operation *op);
  void eraseFromReadLocks(boost::upgrade_mutex* m);
  void releaseLock();
public:
  int status;
  Ss2plTransaction();
  ~Ss2plTransaction();
  void execute(const boost::thread::id tid);
};
