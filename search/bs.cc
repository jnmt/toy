#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define DEFAULT_SIZE 100000

int Size_data;
std::vector<unsigned int> Key_vector;

void print_performance(struct timeval begin, struct timeval end) {
	long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000
                + (end.tv_usec - begin.tv_usec);
	printf("lat:%7ld usec\n", diff);
}

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

void init_vector(void) {
	for (int i = 0; i < Size_data; i++) {
		int key = rand() % 1000000;
		Key_vector.push_back(key);
	}
  std::sort(Key_vector.begin(), Key_vector.end());
}

void binary_search(unsigned int q, int start, int end) {
  if (start > end) {
    printf("Not found: %d\n", q);
    return;
  }
  int middle = start + (end - start)/2;
  if (q > Key_vector[middle]) {
    //printf("%d %d\n", middle+1, end);
    binary_search(q, middle+1, end);
  } else if (q < Key_vector[middle]) {
    //printf("%d %d\n", start, middle-1);
    binary_search(q, start, middle-1);
  } else {
    printf("Found: %d (index: %d)\n", Key_vector[middle], middle);
  }
}

int main(int argc, char *argv[]) {
  Size_data = DEFAULT_SIZE;
  struct timeval begin, end;

	if (argc == 2) Size_data = atoi(argv[1]);
	init_vector();

  // for (int i=0; i<Size_data; i++) {
  //   printf("%d ", Key_vector[i]);
  // }
  // printf("\n");

	unsigned int query;
	std::cout << "Search: ";
	std::cin >> query;
	begin = cur_time();
	binary_search(query, 0, Size_data-1);
	end = cur_time();
	print_performance(begin, end);

	return 0;
}
