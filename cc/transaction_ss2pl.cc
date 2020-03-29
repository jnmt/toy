#include <iostream>
#include <vector>
#include <chrono>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "common.hpp"

Transaction::Transaction() {}

Transaction::~Transaction() {
  for (auto& op : operations) delete op;
  boost::lock_guard<boost::mutex> lock(stdout_mutex);
}

void Transaction::begin(boost::thread::id tid) {
  threadId = tid;
  status = TX_INPROGRESS;
  debug(boost::format("BEGIN"));
}

void Transaction::read(Operation *op) {
  int key = op->key;
  Record* record = getRecord(key);

  if (searchReadSet(key) || searchWriteSet(key))
    goto out;

  if (record->m.try_lock_shared()) {
    debug(boost::format("READ %1% locked") % key);
    readSet.push_back(op);
    readLocks.push_back(&(record->m));
  } else {
    debug(boost::format("READ %1% lock failed") % key);
    abort();
  }

out:
  return;
}

void Transaction::write(Operation *op) {
  int key = op->key;
  Record* record = getRecord(key);
  Operation* rop;
  Operation* wop;

  rop = searchReadSet(key);
  if (rop) {
    if (record->m.try_lock_upgrade()) {
      record->m.unlock_shared();
      eraseFromReadSet(rop);
      eraseFromReadLocks(&(record->m));
      // This may fail if other threads has lock_shared
      if (record->m.try_unlock_upgrade_and_lock()) {
        debug(boost::format("WRITE %1% lock upgraded") % key);
        writeSet.push_back(op);
        writeLocks.push_back(&(record->m));
        goto out;
      } else
        goto abort;
    } else
      goto abort;
  }

  wop = searchWriteSet(key);
  if (wop) {
    eraseFromWriteSet(wop);
    writeSet.push_back(op);
    goto out;
  }

  if (record->m.try_lock()) {
    debug(boost::format("WRITE %1% locked") % key);
    writeSet.push_back(op);
    writeLocks.push_back(&(record->m));
    goto out;
  } else {
    goto abort;
  }

abort:
  debug(boost::format("WRITE %1% lock failed") % key);
  abort();

out:
  return;
}

void Transaction::commit() {
  debug(boost::format("COMMIT"));
  for (auto itr = writeSet.begin(); itr != writeSet.end(); ++itr) {
    table[(*itr)->key]->attr = (*itr)->value;
  }
  releaseLock();
  status = TX_COMMIT;
}

void Transaction::abort() {
  debug(boost::format("ABORT"));
  readSet.clear();
  writeSet.clear();
  releaseLock();
  status = TX_ABORT;
}

Operation* Transaction::searchReadSet(int key) {
  for (auto itr = readSet.begin(); itr != readSet.end(); ++itr) {
    if ((*itr)->key == key)
      return (*itr);
  }
  return NULL;
}

Operation* Transaction::searchWriteSet(int key) {
  for (auto itr = writeSet.begin(); itr != writeSet.end(); ++itr) {
    if ((*itr)->key == key)
      return (*itr);
  }
  return NULL;
}

void Transaction::eraseFromReadSet(Operation* op) {
  for (auto itr = readSet.begin(); itr != readSet.end();) {
    if ((*itr) == op) {
      readSet.erase(itr);
      return;
    }
    else
      ++itr;
  }
}

void Transaction::eraseFromWriteSet(Operation* op) {
  for (auto itr = writeSet.begin(); itr != writeSet.end();) {
    if ((*itr) == op) {
      writeSet.erase(itr);
      return;
    }
    else
      ++itr;
  }
}

void Transaction::eraseFromReadLocks(boost::shared_mutex* m) {
  for (auto itr = readLocks.begin(); itr != readLocks.end();) {
    if ((*itr) == m) {
      readLocks.erase(itr);
      return;
    }
    else
      ++itr;
  }
}

void Transaction::releaseLock() {
  for (auto itr = readLocks.begin(); itr != readLocks.end(); ++itr)
    (*itr)->unlock_shared();
  for (auto itr = writeLocks.begin(); itr != writeLocks.end(); ++itr)
    (*itr)->unlock();
  readLocks.clear();
  writeLocks.clear();
}

void Transaction::generateOperations(int numOperations, int readRatio) {
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

std::vector<Operation*> Transaction::getOperations() {
  return operations;
}

Record* Transaction::getRecord(int key) {
  return table[key];
}

#ifdef _DEBUG
void Transaction::debug(boost::format fmt) {
  boost::lock_guard<boost::mutex> lock(stdout_mutex);
  auto now = std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
  ).count();
  std::cout << now << " [" << threadId << "] " << fmt << std::endl;
}
#else
void Transaction::debug(boost::format fmt) {}
#endif
