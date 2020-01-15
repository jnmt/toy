#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define DEFAULT_DELIMITER '|'
#define MAX_TABLE_NUM 16

typedef struct _RECORD {
  std::vector<int> attr;
} RECORD;

extern int tableSize;
extern int numSpace;
extern std::vector<std::vector<RECORD*>> tables;

RECORD* alloc_record();
RECORD* join_record(RECORD *l, RECORD *r);
