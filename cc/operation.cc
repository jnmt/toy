#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <vector>

#include "common.hpp"

Operation::Operation(int k) {
  type = OP_READ;
  key = k;
}

Operation::Operation(int k, int v) {
  type = OP_WRITE;
  key = k;
  value = v;
}

bool Operation::isRead() {
  if (type == OP_READ)
    return true;
  else
    return false;
}
