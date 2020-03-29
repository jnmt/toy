#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "common.hpp"
#include "transaction.hpp"

std::vector<Record*> table;
std::vector<std::vector<Transaction*>> tasks;
std::vector<Transaction*> txtask(1000);

boost::mutex stdout_mutex;

void worker(std::vector<Transaction*> task) {
  for (auto& transaction : task)
    transaction->execute(boost::this_thread::get_id());
}

Record* alloc_record() {
  Record *record = new Record();
  record->attr = 0;
  return record;
}

void init_database(int n) {
  for (int i = 0; i < n; i++) {
    table.push_back(alloc_record());
  }
}

Transaction* generate_transaction(int mode, int numOps, int readRatio) {
  Transaction* tx;
  switch (mode) {
    case MODE_S2PL:
      tx = (Transaction*)new Ss2plTransaction();
      break;
    default:
      tx = (Transaction*)new BaseTransaction();
  }
  tx->generateOperations(numOps, readRatio);
  return (Transaction*)tx;
}

void init_tasks(int mode, int numThreads, int numTransactions,
                int numOps, int readRatio) {
  for (int i = 0; i < numThreads; i++) {
    std::vector<Transaction*> task;
    tasks.push_back(task);
  }
  for (int i = 0; i < numTransactions; i++) {
    tasks[i%numThreads].push_back(
      generate_transaction(mode, numOps, readRatio));
  }
}

int main(int argc, char *argv[]) {
  struct timeval begin, end;
  std::vector<boost::thread> threads;

  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
    ("help,h",
      "Show this message")
    ("mode,m",
      po::value<std::string>()->default_value("ss2pl"),
      "Set concurrency control mode\n(ss2pl or occ)")
    ("num-threads,p",
      po::value<int>()->default_value(DEFAULT_NUM_THREADS),
      "Set number of threads")
    ("table-size,t",
      po::value<int>()->default_value(DEFAULT_TABLE_SIZE),
      "Set number of records in the table")
    ("num-transactions,n",
      po::value<int>(),
      "Set number of transactions to run")
    ("num-operations,o",
      po::value<int>()->default_value(DEFAULT_NUM_OPERATIONS),
      "Set number of operation in transaction")
    ("read-ratio,r",
      po::value<int>(),
      "Set read ratio (0-100)")
    ;

  po::variables_map argmap;
  po::store(po::parse_command_line(argc, argv, desc), argmap);
  po::notify(argmap);

  if (argmap.count("help")
      || !argmap.count("num-transactions") || !argmap.count("read-ratio")) {
    std::cout << desc << std::endl;
    return 1;
  }

  std::string mode = argmap["mode"].as<std::string>();
  int tableSize = argmap["table-size"].as<int>();
  int numThreads = argmap["num-threads"].as<int>();
  int numTxns = argmap["num-transactions"].as<int>();
  int numOps = argmap["num-operations"].as<int>();
  int readRatio = argmap["read-ratio"].as<int>();

  std::cout << "Starting with following parameters" << std::endl
            << "  Mode: " << mode << std::endl
            << "  Threads: " << numThreads << std::endl
            << "  Records in table: " << tableSize << std::endl
            << "  Number of transactions to run: " << numTxns << std::endl
            << "  Number of operation in transaction: " << numOps << std::endl
            << "  Read: " << readRatio << "%" << std::endl;

  init_database(tableSize);
  init_tasks(MODE_S2PL, numThreads, numTxns, numOps, readRatio);

  begin = cur_time();
  for (int i = 0; i < numThreads; i++) {
    // TODO: Learn difference between push_back+move and emplace_back
    boost::thread t(worker, tasks[i]);
    threads.push_back(std::move(t));
    //threads.emplace_back(worker, tasks[i]);
  }
  for (auto& thread : threads) thread.join();

  end = cur_time();
  print_performance(numTxns, begin, end);

  return 0;
}
