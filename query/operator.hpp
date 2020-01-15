#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "common.hpp"

/* Operators */
class Op {
public:
  virtual void open();
  virtual void close();
  virtual RECORD* next();
  virtual int num_attrs();
};

class ScanOp : Op {
private:
  std::vector<RECORD*> table;
  int cursor;
  int size;

public:
  ScanOp(std::vector<RECORD*> t);
  void open();
  void close();
  RECORD* next();
  int num_attrs();
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
  NestedLoopJoinOp(Op *l, Op *r, int lattr, int rattr);
  void open();
  void close();
  RECORD* next();
  int num_attrs();
};
