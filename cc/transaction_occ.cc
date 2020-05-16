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

/* GC */
#define OCC_GC_THRESHOLD 10000
std::atomic<int> deletedTxId(-1);
std::map<boost::thread::id,int> progress;

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
      gc();
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
  int begin = startTxId - (deletedTxId.load() + 1);
  int end = txId.load() - (deletedTxId.load() + 1);
  for (int i = begin; i < end; i++) {
    debug(boost::format("Validating tx #%1%") % (i + deletedTxId.load() - 1));
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
  debug(boost::format("COMMIT tx #%1%") % txId.load());
  for (const auto& [key, record] : writeSet) {
    table[key]->attr = record->attr;
  }
  status = TX_COMMIT;
  txns.push_back(this);
  progress[threadId] = txId.load();
  txId++;
}

void OccTransaction::abort() {
  debug(boost::format("ABORT"));
  readSet.clear();
  writeSet.clear();
  status = TX_ABORT;
}

void OccTransaction::gc() {
  if (txns.size() > OCC_GC_THRESHOLD) {
    auto slowest = std::min_element(progress.begin(), progress.end(),
      [](const std::pair<boost::thread::id,int>& p1,
         const std::pair<boost::thread::id,int>& p2) {
        return p1.second < p2.second;
      });
    int numTxnsDeleted = slowest->second - deletedTxId.load();
    if (numTxnsDeleted > 0) {
      debug(boost::format("GC transactions <= #%1%") % slowest->second);
      txns.erase(txns.begin(), txns.begin() + numTxnsDeleted);
      deletedTxId = slowest->second;
    }
  }
}
