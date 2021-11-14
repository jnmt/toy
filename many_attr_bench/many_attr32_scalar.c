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
  bool *x08 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x09 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x10 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x11 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x12 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x13 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x14 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x15 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x16 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x17 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x18 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x19 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x20 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x21 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x22 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x23 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x24 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x25 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x26 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x27 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x28 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x29 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x30 = (bool *)malloc(268435456 * sizeof(bool));
  bool *x31 = (bool *)malloc(268435456 * sizeof(bool));
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
    x08[i] = rand()%100 < selectivity ? true : false;
    x09[i] = rand()%100 < selectivity ? true : false;
    x10[i] = rand()%100 < selectivity ? true : false;
    x11[i] = rand()%100 < selectivity ? true : false;
    x12[i] = rand()%100 < selectivity ? true : false;
    x13[i] = rand()%100 < selectivity ? true : false;
    x14[i] = rand()%100 < selectivity ? true : false;
    x15[i] = rand()%100 < selectivity ? true : false;
    x16[i] = rand()%100 < selectivity ? true : false;
    x17[i] = rand()%100 < selectivity ? true : false;
    x18[i] = rand()%100 < selectivity ? true : false;
    x19[i] = rand()%100 < selectivity ? true : false;
    x20[i] = rand()%100 < selectivity ? true : false;
    x21[i] = rand()%100 < selectivity ? true : false;
    x22[i] = rand()%100 < selectivity ? true : false;
    x23[i] = rand()%100 < selectivity ? true : false;
    x24[i] = rand()%100 < selectivity ? true : false;
    x25[i] = rand()%100 < selectivity ? true : false;
    x26[i] = rand()%100 < selectivity ? true : false;
    x27[i] = rand()%100 < selectivity ? true : false;
    x28[i] = rand()%100 < selectivity ? true : false;
    x29[i] = rand()%100 < selectivity ? true : false;
    x30[i] = rand()%100 < selectivity ? true : false;
    x31[i] = rand()%100 < selectivity ? true : false;
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
    bool cond08;
    if (cond07) {
      cond08 = x08[i] == false;
    } else {
      cond08 = false;
    }
    bool cond09;
    if (cond08) {
      cond09 = x09[i] == false;
    } else {
      cond09 = false;
    }
    bool cond10;
    if (cond09) {
      cond10 = x10[i] == false;
    } else {
      cond10 = false;
    }
    bool cond11;
    if (cond10) {
      cond11 = x11[i] == false;
    } else {
      cond11 = false;
    }
    bool cond12;
    if (cond11) {
      cond12 = x12[i] == false;
    } else {
      cond12 = false;
    }
    bool cond13;
    if (cond12) {
      cond13 = x13[i] == false;
    } else {
      cond13 = false;
    }
    bool cond14;
    if (cond13) {
      cond14 = x14[i] == false;
    } else {
      cond14 = false;
    }
    bool cond15;
    if (cond14) {
      cond15 = x15[i] == false;
    } else {
      cond15 = false;
    }
    bool cond16;
    if (cond15) {
      cond16 = x16[i] == false;
    } else {
      cond16 = false;
    }
    bool cond17;
    if (cond16) {
      cond17 = x17[i] == false;
    } else {
      cond17 = false;
    }
    bool cond18;
    if (cond17) {
      cond18 = x18[i] == false;
    } else {
      cond18 = false;
    }
    bool cond19;
    if (cond18) {
      cond19 = x19[i] == false;
    } else {
      cond19 = false;
    }
    bool cond20;
    if (cond19) {
      cond20 = x20[i] == false;
    } else {
      cond20 = false;
    }
    bool cond21;
    if (cond20) {
      cond21 = x21[i] == false;
    } else {
      cond21 = false;
    }
    bool cond22;
    if (cond21) {
      cond22 = x22[i] == false;
    } else {
      cond22 = false;
    }
    bool cond23;
    if (cond22) {
      cond23 = x23[i] == false;
    } else {
      cond23 = false;
    }
    bool cond24;
    if (cond23) {
      cond24 = x24[i] == false;
    } else {
      cond24 = false;
    }
    bool cond25;
    if (cond24) {
      cond25 = x25[i] == false;
    } else {
      cond25 = false;
    }
    bool cond26;
    if (cond25) {
      cond26 = x26[i] == false;
    } else {
      cond26 = false;
    }
    bool cond27;
    if (cond26) {
      cond27 = x27[i] == false;
    } else {
      cond27 = false;
    }
    bool cond28;
    if (cond27) {
      cond28 = x28[i] == false;
    } else {
      cond28 = false;
    }
    bool cond29;
    if (cond28) {
      cond29 = x29[i] == false;
    } else {
      cond29 = false;
    }
    bool cond30;
    if (cond29) {
      cond30 = x30[i] == false;
    } else {
      cond30 = false;
    }
    bool cond31;
    if (cond30) {
      cond31 = x31[i] == false;
    } else {
      cond31 = false;
    }
    if (cond31) {
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
