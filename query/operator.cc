#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>

#include "common.hpp"
#include "operator.hpp"

void Op::open() {}
void Op::close() {}
RECORD* Op::next() { return NULL; }
int Op::num_attrs() { return 0; }

ScanOp::ScanOp(std::vector<RECORD *> t) { table = t; }

void ScanOp::open() {
  cursor = 0;
  size = table.size();
}

void ScanOp::close() {}

RECORD *ScanOp::next() {
  if (cursor < size) {
    return table[cursor++];
  } else
    return NULL;
}

int ScanOp::num_attrs() {
  // This should be independent to the record instance
  return table[0]->attr.size();
}

NestedLoopJoinOp::NestedLoopJoinOp(Op *l, Op *r, int lattr, int rattr) {
  left = l;
  right = r;
  leftAttr = lattr;
  rightAttr = rattr;
}

void NestedLoopJoinOp::open() {
  left->open();
  right->open();
  leftRec = left->next();
}

void NestedLoopJoinOp::close() {
  left->close();
  right->close();
}

RECORD *NestedLoopJoinOp::next() {
  while (leftRec) {
    rightRec = right->next();
    while (rightRec) {
      if (leftRec->attr[leftAttr] == rightRec->attr[rightAttr])
        return join_record(leftRec, rightRec);
      else
        rightRec = right->next();
    }
    right->close();
    right->open();
    leftRec = left->next();
  }
  return NULL;
}

int NestedLoopJoinOp::num_attrs() {
  return left->num_attrs()+right->num_attrs();
}
