#include <iostream>
#include <vector>
#include <chrono>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "common.hpp"
#include "transaction.hpp"

Ss2plTransaction::Ss2plTransaction() {}

Ss2plTransaction::~Ss2plTransaction() {
  for (auto& op : operations) delete op;
  boost::lock_guard<boost::mutex> lock(stdout_mutex);
}

void Ss2plTransaction::execute(boost::thread::id tid) {
  threadId = tid;
  retry:
  begin();
  for (auto& operation : getOperations()) {
    if (operation->isRead())
      read(operation);
    else
      write(operation);
    if (status == TX_ABORT) {
      usleep(3000); // to avoid resource contention
      goto retry;
    }
  }
  commit();
}

void Ss2plTransaction::begin() {
  status = TX_INPROGRESS;
  debug(boost::format("BEGIN"));
}

void Ss2plTransaction::read(Operation *op) {
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

void Ss2plTransaction::write(Operation *op) {
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

void Ss2plTransaction::commit() {
  debug(boost::format("COMMIT"));
  for (auto itr = writeSet.begin(); itr != writeSet.end(); ++itr) {
    table[(*itr)->key]->attr = (*itr)->value;
  }
  releaseLock();
  status = TX_COMMIT;
}

void Ss2plTransaction::abort() {
  debug(boost::format("ABORT"));
  readSet.clear();
  writeSet.clear();
  releaseLock();
  status = TX_ABORT;
}

void Ss2plTransaction::eraseFromReadLocks(boost::shared_mutex* m) {
  for (auto itr = readLocks.begin(); itr != readLocks.end();) {
    if ((*itr) == m) {
      readLocks.erase(itr);
      return;
    }
    else
      ++itr;
  }
}

void Ss2plTransaction::releaseLock() {
  for (auto itr = readLocks.begin(); itr != readLocks.end(); ++itr)
    (*itr)->unlock_shared();
  for (auto itr = writeLocks.begin(); itr != writeLocks.end(); ++itr)
    (*itr)->unlock();
  readLocks.clear();
  writeLocks.clear();
}
