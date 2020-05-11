#include <iostream>
#include <vector>
#include <chrono>

#include <boost/format.hpp>
#include <boost/thread.hpp>

#include "common.hpp"
#include "transaction.hpp"

std::atomic<int> txId(0);
boost::mutex valwrite_mutex;
std::vector<OccTransaction*> txns;

OccTransaction::OccTransaction() {}

OccTransaction::~OccTransaction() {
  for (auto& op : operations) delete op;
}

void OccTransaction::execute(boost::thread::id tid) {
  threadId = tid;
  retry:
  begin();
  for (auto& operation : getOperations()) {
    if (operation->isRead())
      read(operation);
    else
      write(operation);
  }
  // Validation and commit
  {
    boost::mutex::scoped_lock scoped_lock(valwrite_mutex);
    if (validate()) {
      commit();
    } else {
      abort();
      goto retry;
    }
  }
}

void OccTransaction::begin() {
  debug(boost::format("BEGIN"));
  status = TX_INPROGRESS;
  startTxId = txId.load();
}

void OccTransaction::read(Operation *op) {
  int key = op->key;
  OccRecord* record = (OccRecord*)getRecord(key);
  OccRecord newCopy;

  if (!searchReadSet(key) || !searchWriteSet(key)) {
    debug(boost::format("READ %1%") % key);
    newCopy = *record;
    readSet.emplace(key, &newCopy);
  }

  return;
}

void OccTransaction::write(Operation *op) {
  int key = op->key;
  OccRecord* record = (OccRecord*)getRecord(key);
  OccRecord* localCopy;
  OccRecord newCopy;

  localCopy = (OccRecord*)searchReadSet(key);
  if (localCopy) {
    debug(boost::format("WRITE %1% %2%") % key % op->value);
    localCopy->attr = op->value;
    readSet.erase(key);
    writeSet.emplace(key, localCopy); // This may cause SIGSEGV on Mac x Clang
    goto out;
  }

  localCopy = (OccRecord*)searchWriteSet(key);
  if (localCopy) {
    debug(boost::format("WRITE %1% %2%") % key % op->value);
    localCopy->attr = op->value;
    goto out;
  }

  //newCopy = OccRecord(*record);
  debug(boost::format("WRITE %1% %2%") % key % op->value);
  newCopy = *record;
  newCopy.attr = op->value;
  writeSet.emplace(key, &newCopy);

out:
  return;
}

bool OccTransaction::validate() {
  int finishTxId = txId.load();
  for (int i = startTxId; i < finishTxId; i++) {
    debug(boost::format("Validating tx #%1%") % i);
    for (const auto& [key, record] : readSet) {
      if (txns[i]->searchWriteSet(key)) {
        debug(boost::format("Conflict with %1%, %2%") % key % record);
        return false;
      }
    }
  }
  return true;
}

void OccTransaction::commit() {
  debug(boost::format("COMMIT"));
  for (const auto& [key, record] : writeSet) {
    table[key]->attr = record->attr;
  }
  status = TX_COMMIT;
  txns.push_back(this);
  txId++;
}

void OccTransaction::abort() {
  debug(boost::format("ABORT"));
  readSet.clear();
  writeSet.clear();
  status = TX_ABORT;
}
