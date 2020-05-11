#include <iostream>
#include <vector>
#include <chrono>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "common.hpp"
#include "transaction.hpp"

BaseTransaction::BaseTransaction() {}

BaseTransaction::~BaseTransaction() {
  for (auto& op : operations) delete op;
  boost::lock_guard<boost::mutex> lock(stdout_mutex);
}

void BaseTransaction::execute(boost::thread::id tid) {
  // Example implementation without cc
  begin();
  for (auto& operation : getOperations()) {
    if (operation->isRead()) {
      debug(boost::format("READ %1%") % operation->key);
      read(operation);
    } else {
      debug(boost::format("WRITE %1%") % operation->key);
      write(operation);
    }
  }
  commit();
}

void BaseTransaction::begin() { status = TX_INPROGRESS; }
void BaseTransaction::commit() { status = TX_COMMIT; }
void BaseTransaction::abort() { status = TX_ABORT; }
void BaseTransaction::read(Operation *op) {}
void BaseTransaction::write(Operation *op) {}

Record* BaseTransaction::searchReadSet(int k) {
  for (const auto& [key, record] : readSet) {
    if (key == k)
      return record;
  }
  return NULL;
}

Record* BaseTransaction::searchWriteSet(int k) {
  for (const auto& [key, record] : writeSet) {
    if (key == k)
      return record;
  }
  return NULL;
}

void BaseTransaction::generateOperations(int numOperations, int readRatio) {
  Operation* op;
  int type, key, value;
  auto seed = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
  ).count();
  srand(seed);
  for (int i = 0; i < numOperations; i++) {
    type = rand() % 100 < readRatio ? OP_READ : OP_WRITE;
    key = rand() % table.size();
    if (type == OP_READ) {
      op = new Operation(key);
    } else {
      value = rand();
      op = new Operation(key, value);
    }
    operations.push_back(op);
  }
}

std::vector<Operation*> BaseTransaction::getOperations() {
  return operations;
}

Record* BaseTransaction::getRecord(int key) {
  return table[key];
}

#ifdef _DEBUG
void BaseTransaction::debug(boost::format fmt) {
  boost::lock_guard<boost::mutex> lock(stdout_mutex);
  auto now = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
  ).count();
  std::cout << now << " [" << threadId << "] " << fmt << std::endl;
}
#else
void BaseTransaction::debug(boost::format fmt) {}
#endif
