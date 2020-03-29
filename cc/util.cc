#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "common.hpp"

struct timeval cur_time(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t;
}

void print_performance(int numTxns, struct timeval begin, struct timeval end) {
  printf("All transactions completed\n");
  long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000
            + (end.tv_usec - begin.tv_usec);
  if (diff > 1000) {
    printf("  Execution time: %ld.%ld ms\n", diff/1000, diff%1000);
    printf("  Throughput: %ld tps\n", (numTxns*1000)/(diff/1000));
  } else {
    printf("  Execution time: 0.%ld ms\n", diff);
    printf("  Throughput: N/A (Execution time too short)\n");
  }
}
