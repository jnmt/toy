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
    __m512i x = _mm512_set_epi8(
      x00[i], x01[i], x02[i], x03[i], x04[i], x05[i], x06[i], x07[i],
      x08[i], x09[i], x10[i], x11[i], x12[i], x13[i], x14[i], x15[i],
      x16[i], x17[i], x18[i], x19[i], x20[i], x21[i], x22[i], x23[i],
      x24[i], x25[i], x26[i], x27[i], x28[i], x29[i], x30[i], x31[i],
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true
    );
    __m512i y = _mm512_set_epi8(
      false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false,
      false, false, false, false, false, false, false, false,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true
    );
    __mmask64 mask = _mm512_cmpeq_epi8_mask(x, y);
    int32_t result = _mm_popcnt_u64(mask);
    if (result == 64) {
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
