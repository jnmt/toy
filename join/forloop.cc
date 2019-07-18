#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <vector>

#define DEFAULT_TABLE_SIZE 1000
#define DEFAULT_NUM_SPACE 10000
#define MAX_TABLE_NUM 16

void print_performance(struct timeval begin, struct timeval end) {
  long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000
                + (end.tv_usec - begin.tv_usec);
  printf("lat:%7ld usec\n", diff);
}

struct timeval
cur_time(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t;
}

typedef struct _RECORD {
  std::vector<int> attr;
} RECORD;

int tableSize;
int numSpace;
std::vector<std::vector<RECORD*>> tables(MAX_TABLE_NUM);

RECORD* alloc_record() {
  RECORD *record;
  if (!(record = (RECORD *)calloc(1, sizeof(RECORD))))
    printf("Error (%s): calloc failed.\n", __func__);
  record->attr.push_back(rand() % numSpace); // key for join
  record->attr.push_back(0); // value
  return record;
}

RECORD* join_record(RECORD *l, RECORD *r) {
  RECORD *record;
  if (!(record = (RECORD *)calloc(1, sizeof(RECORD))))
    printf("Error (%s): calloc failed.\n", __func__);

  for (long unsigned int i = 0; i < l->attr.size(); i++) {
    record->attr.push_back(l->attr[i]);
  }

  for (long unsigned int j = 0; j < r->attr.size(); j++) {
    record->attr.push_back(r->attr[j]);
  }

  return record;
}

void init_database(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < tableSize; j++) {
      tables[i].push_back(alloc_record());
    }
  }
}

void print_database(int n) {
  for (int i = 0; i < n; i++) {
    for (long unsigned int j = 0; j < tables[i].size(); j++) {
      printf("[Table#: %d, Row#: %ld] %d %d\n",
        i, j, tables[i][j]->attr[0], tables[i][j]->attr[1]);
    }
  }
}

std::vector<RECORD*>* simple_nested_loop_join(
  std::vector<RECORD*>& right, std::vector<RECORD*>& left) {
  std::vector<RECORD*> *res;
  RECORD *leftRec;
  RECORD *rightRec;
  if (!(res = (std::vector<RECORD*>*)calloc(1, sizeof(std::vector<RECORD*>))))
    printf("Error (%s): calloc failed.\n", __func__);

  for (long unsigned int i=0; i < left.size(); i++) {
    leftRec = left[i];
    for (long unsigned int j=0; j < right.size(); j++) {
      rightRec = right[j];
      if (leftRec->attr[0] == rightRec->attr[0]) {
        res->push_back(join_record(leftRec, rightRec));
      }
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  tableSize = DEFAULT_TABLE_SIZE;
  numSpace = DEFAULT_NUM_SPACE;
  struct timeval begin, end;

  if (argc >= 2) tableSize = atoi(argv[1]);
  if (argc == 3) numSpace = atoi(argv[2]);

  unsigned int n;
  std::cout << "Number of tables: ";
  std::cin >> n;

  if (n > MAX_TABLE_NUM || n < 2) {
    printf("Invalid number of tables: %d\n", n);
  } else {
    init_database(n);
    //print_database(n);

    // Execute join
    begin = cur_time();
    std::vector<RECORD*> *tmp;
    tmp = simple_nested_loop_join(tables[0], tables[1]);
    for (unsigned int i=2; i < n; i++) {
      tmp = simple_nested_loop_join(*tmp, tables[i]);
    }
    end = cur_time();
    printf("Number of matched rows: %ld\n", tmp->size());
    print_performance(begin, end);
  }

  return 0;
}
