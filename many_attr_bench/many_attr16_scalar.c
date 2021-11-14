#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <immintrin.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h> 
#include <unistd.h>
#define NUM_RECORDS 1000000
#define SELECTIVITY 5

void print_performance(struct timeval begin, struct timeval end) {
  long diff =
      (end.tv_sec - begin.tv_sec) * 1000 * 1000 + (end.tv_usec - begin.tv_usec);
  if (diff > 1000) {
    printf("%ld.%ld ms\n", diff / 1000, diff % 1000);
  } else {
    printf("0.%ld ms\n", diff);
  }
}
struct timeval cur_time(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t;
}
void Snippet(int32_t n, int32_t selectivity);
int main(int argc, char *argv[]) {
  int32_t n, s;
  if (argc == 3) {
    n = atoi(argv[1]);
    s = atoi(argv[2]);
  } else {
    n = NUM_RECORDS;
    s = SELECTIVITY;
  }
  Snippet(n, s);
  return 0;
}

/*****************************************
  Emitting C Generated Code
*******************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Snippet(int32_t num, int32_t selectivity) {
  bool *x00 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x01 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x02 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x03 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x04 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x05 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x06 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x07 = (bool *)malloc(268435456 * sizeof(bool));
  int32_t count = 0;
  // creating test database
  srand((unsigned)time(NULL));
  for (int i = 0; i < num; i++) {
    x00[i] = rand()%100 < selectivity ? true : false;
    x01[i] = rand()%100 < selectivity ? true : false;
    x02[i] = rand()%100 < selectivity ? true : false;
    x03[i] = rand()%100 < selectivity ? true : false;
    x04[i] = rand()%100 < selectivity ? true : false;
    x05[i] = rand()%100 < selectivity ? true : false;
    x06[i] = rand()%100 < selectivity ? true : false;
    x07[i] = rand()%100 < selectivity ? true : false;
  }
  struct timeval begin, end;
  begin = cur_time();
  for (int i = 0; i < num; i++) {
    bool cond00 = x00[i] == false;
    bool cond01;
    if (cond00) {
      cond01 = x01[i] == false;
    } else {
      cond01 = false;
    }
    bool cond02;
    if (cond01) {
      cond02 = x02[i] == false;
    } else {
      cond02 = false;
    }
    bool cond03;
    if (cond02) {
      cond03 = x03[i] == false;
    } else {
      cond03 = false;
    }
    bool cond04;
    if (cond03) {
      cond04 = x04[i] == false;
    } else {
      cond04 = false;
    }
    bool cond05;
    if (cond04) {
      cond05 = x05[i] == false;
    } else {
      cond05 = false;
    }
    bool cond06;
    if (cond05) {
      cond06 = x06[i] == false;
    } else {
      cond06 = false;
    }
    bool cond07;
    if (cond06) {
      cond07 = x07[i] == false;
    } else {
      cond07 = false;
    }
    if (cond07) {
      count++;
    }
  }
  end = cur_time();
  printf("Matched: %d\n", count);
  print_performance(begin, end);
}
/*****************************************
  End of C Generated Code
*******************************************/
