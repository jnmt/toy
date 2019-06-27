#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <vector>

#define HASH_SIZE 4096
#define DEFAULT_SIZE 100000

typedef struct _DATA {
	int val;
	struct _DATA *next;
} DATA;

typedef struct _ENTRY {
	struct _DATA *next;
} ENTRY;

int Size_data;
std::vector<ENTRY*> HashTable;

void print_performance(struct timeval begin, struct timeval end) {
	long diff = (end.tv_sec - begin.tv_sec) * 1000 * 1000
                + (end.tv_usec - begin.tv_usec);
	printf("lat:%7ld usec\n", diff);
}

struct timeval cur_time(void) {
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

DATA * alloc_data(int val) {
	DATA *data;
	if (!(data = (DATA *)calloc(1, sizeof(DATA))))
		printf("Error (%s): calloc failed.\n", __func__);;
	data->val = val;
	data->next = NULL;
	//printf("%p %d\n", data, val);
	return data;
}

ENTRY * alloc_entry() {
	ENTRY *entry;
	if (!(entry = (ENTRY *)calloc(1, sizeof(ENTRY))))
		printf("Error (%s): calloc failed.\n", __func__);;
	entry->next = NULL;
	return entry;
}

void add_value(int val) {
	int key = val % HASH_SIZE;
	DATA *data = (DATA *)HashTable[key];
	DATA *current = data;
	while (data) {
		current = data;
		data = data->next;
	}
	current->next = alloc_data(val);
}

void init_hash_table(void) {
	for (int i = 0; i < HASH_SIZE; i++) {
		HashTable.push_back(alloc_entry());
	}
	for (int i = 0; i < Size_data; i++) {
		int val = rand() % 1000000;
		add_value(val);
	}
}

void search_value(int val) {
	int found = 0;
	int key = val % HASH_SIZE;
	DATA *data = (DATA *)HashTable[key];
	//printf("Hash key: %d\n", key);
	while (data->next) {
		//printf("\tValue: %d\n", data->next->val);
		if (data->next->val == val) {
			printf("Found: %d (hash key: %d)\n", val, key);
			found++;
		}
		data = data->next;
	}
	if (!found)
		printf("Not found\n");
}

int main(int argc, char *argv[]) {
  Size_data = DEFAULT_SIZE;
  struct timeval begin, end;

	if (argc == 2) Size_data = atoi(argv[1]);
	init_hash_table();

	unsigned int query;
	std::cout << "Search: ";
	std::cin >> query;
	begin = cur_time();
	search_value(query);
	end = cur_time();
	print_performance(begin, end);

	return 0;
}
