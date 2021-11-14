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
    __m512i x = _mm512_set_epi8(
      x00[i], x01[i], x02[i], x03[i], x04[i], x05[i], x06[i], x07[i],
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true
    );
    __m512i y = _mm512_set_epi8(
      false, false, false, false, false, false, false, false,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true,
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
