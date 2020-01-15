#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <vector>

#include "common.hpp"
#include "operator.hpp"

void print_performance(struct timeval begin, struct timeval end) {
  long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000
            + (end.tv_usec - begin.tv_usec);
  if (diff > 1000) {
    printf("%ld.%ld ms\n", diff/1000, diff%1000);
  } else {
    printf("0.%ld ms\n", diff);
  }
}

struct timeval
cur_time(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t;
}

int tableSize;
int numSpace;
std::vector<std::vector<RECORD*>> tables(MAX_TABLE_NUM);

RECORD* alloc_record(int id) {
  RECORD *record;
  if (!(record = (RECORD *)calloc(1, sizeof(RECORD))))
    printf("Error (%s): calloc failed.\n", __func__);
  return record;
}

RECORD* join_record(RECORD *l, RECORD *r) {
  for (long unsigned int j = 0; j < r->attr.size(); j++) {
    l->attr.push_back(r->attr[j]);
  }
  return l;
}

void init_database(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < tableSize; j++) {
      tables[i].push_back(alloc_record(j));
    }
  }
}

bool load_table(std::vector<RECORD*> *t, const char *filename) {
  std::ifstream filestream(filename);
  if (!filestream.is_open()) return false;
  int num = 0;
  while (!filestream.eof()) {
    std::string buffer;
    filestream >> buffer;

    RECORD* record = alloc_record(num);
    std::istringstream streambuffer(buffer);
    std::string token;
    while (std::getline(streambuffer, token, DEFAULT_DELIMITER)) {
      record->attr.push_back(atoi(token.c_str()));
    }
    if (record->attr.size() > 0) {
      t->push_back(record);
      num++;
    }
  }
  return true;
}

void print_database(int n) {
  for (int i = 0; i < n; i++) {
    for (long unsigned int j = 0; j < tables[i].size(); j++) {
      printf("[Table#: %d, Row#: %ld] ", i, j);
      for (long unsigned int k = 0; k < tables[i][j]->attr.size(); k++)
        printf("%d ", tables[i][j]->attr[k]);
      printf("\n");
    }
  }
}

Op* create_op_tree(Op *left, Op *right) {
  NestedLoopJoinOp *nlj = new NestedLoopJoinOp(
                                (Op*)left,
                                (Op*)right,
                                ((Op*)left)->num_attrs()-1, 0);
  return (Op*)nlj;
}

int main(int argc, char *argv[]) {
  struct timeval begin, end;

  unsigned int n = argc - 1;
  if (n < 2 || n > MAX_TABLE_NUM) {
    printf("Invalid number of tables: %d\n", n);
    exit(1);
  }

  for (unsigned int i=0; i < n; i++) {
    load_table(&tables[i], argv[i+1]);
  }
  // init_database(n);
  // print_database(n);

  // Create tree of operators
  std::vector<ScanOp*> scan(n);
  for (unsigned int i=0; i < n; i++) {
     scan[i] = new ScanOp(tables[i]);
  }
  Op *tree = create_op_tree((Op*)scan[0], (Op*)scan[1]);
  for (unsigned int i=2; i < n; i++) {
    tree = create_op_tree(tree, (Op*)scan[i]);
  }

  // Execute join
  begin = cur_time();
  tree->open();
  int count = 0;
  while (1) {
    RECORD *rec = tree->next();
    if (rec) {
      count++;
      // for (long unsigned int i = 0; i < rec->attr.size(); i++)
      //   printf("%d ", rec->attr[i]);
      // printf("\n");
    } else {
      break;
    }
  }
  tree->close();
  end = cur_time();
  // printf("Number of matched rows: %d\n", count);
  print_performance(begin, end);

  return 0;
}
