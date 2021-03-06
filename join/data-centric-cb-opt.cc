#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <functional>
#include <typeinfo>
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

class Op {
public:
  Op* parent;

public:
  virtual void open() {}
  virtual void close() {}
  virtual std::function<void(std::function<void(RECORD*)>)> exec() {
    return NULL;
  }
};

class ScanOp : Op {
private:
  std::vector<RECORD*> table;
  int cursor;
  int size;

public:
  ScanOp(std::vector<RECORD*> t){
    table = t;
  }

public:
  void open() {
    cursor = 0;
    size = table.size();
  }

public:
  void close() {
  }

public:
  std::function<void(std::function<void(RECORD*)>)> exec() {
    return [&](std::function<void(RECORD*)> callback){
      while (cursor < size) {
        callback(table[cursor++]);
      }
    };
  }
};

class NestedLoopJoinOp : Op {
private:
  Op *left;
  Op *right;
  int leftAttr;
  int rightAttr;
  RECORD *leftRec;
  RECORD *rightRec;

public:
  NestedLoopJoinOp(Op *l, Op *r, int lattr, int rattr) {
    left = l;
    right = r;
    leftAttr = lattr;
    rightAttr = rattr;
  }

public:
  void open() {
    left->parent = this;
    right->parent = this;
    left->open();
    right->open();
  }

public:
  void close() {
    left->close();
    right->close();
  }

public:
  std::function<void(std::function<void(RECORD*)>)> exec() {
    auto dataloopL = left->exec();
    auto dataloopR = right->exec();
    return [this,dataloopL,dataloopR](std::function<void(RECORD*)> callback) {
      dataloopL([&](RECORD* rec) {
        leftRec = rec;
        dataloopR([&](RECORD* rec) {
          rightRec = rec;
          if (leftRec->attr[leftAttr] == rightRec->attr[rightAttr])
            callback(join_record(leftRec, rightRec));
        });
        right->close();
        right->open();
      });
    };
  }
};

class PrintOp : Op {
private:
  Op* child;
  int counter;

public:
  PrintOp(Op *op){
    child = op;
    counter = 0;
  }

public:
  void open() {
    child->parent = this;
    child->open();
  }

public:
  void close() {
    child->close();
  }

public:
  std::function<void(std::function<void(RECORD*)>)> exec() {
    auto loop = child->exec();
    loop([&](RECORD* rec) {
      // for (long unsigned int i = 0; i < rec->attr.size(); i++)
      //   printf("%d ", rec->attr[i]);
      // printf("\n");
      counter++;
    });
    return NULL;
  }

public:
  int getCount() {
    return counter;
  }
};

Op* create_op_tree(Op *left, Op *right) {
  NestedLoopJoinOp *nlj = new NestedLoopJoinOp((Op*)left, (Op*)right, 0, 0);
  return (Op*)nlj;
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

    // Create tree of operators
    std::vector<ScanOp*> scan(n);
    for (unsigned int i=0; i < n; i++) {
       scan[i] = new ScanOp(tables[i]);
    }
    Op *tree = create_op_tree((Op*)scan[0], (Op*)scan[1]);
    for (unsigned int i=2; i < n; i++) {
      tree = create_op_tree(tree, (Op*)scan[i]);
    }
    PrintOp *print = new PrintOp((Op*)tree);

    begin = cur_time();
    print->open();
    print->exec();
    print->close();
    end = cur_time();
    printf("Number of matched rows: %d\n", print->getCount());
    print_performance(begin, end);
  }

  return 0;
}
