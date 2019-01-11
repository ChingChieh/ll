#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "ll.h"

#define DEFAULT_THREAD_NUM 1 
ll_t *list;
void *test(void *ptr) {
    ll_insert_first(list, &((uint32_t*)ptr)[3]); 
	ll_insert_first(list, &((uint32_t*)ptr)[2]);
	ll_insert_last(list, &((uint32_t*)ptr)[1]);
	ll_insert_last(list, &((uint32_t*)ptr)[4]);
	ll_insert_first(list, &((uint32_t*)ptr)[0]);
	return NULL;
}

int main(int argc, char** argv){
    int num_threads = DEFAULT_THREAD_NUM;
	if (argc == 2){
		num_threads = atoi(argv[1]);	
	}
	// Initialize global list 
    list = ll_new(num_teardown);
    list->val_printer = num_printer;
    // Start to create threads 
    pthread_t *threads;
    if ((threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t))) == NULL){
        perror("malloc");
        exit(1);
    }
	// Testing array & empty array use for checking correctness
	uint32_t num_array[5] = {0,1,2,3,4};
	uint32_t check[5] = {};

    for (int i = 0; i < num_threads; ++i) {
		if(pthread_create(&threads[i], NULL, test, (void*)num_array) != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }
	
    for (int i = 0; i < num_threads; ++i) {
        if(pthread_join(threads[i], NULL) != 0){
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
    }
   	 
	ll_print(*list);
	ll_node_t *it = list->hd;
	for (int i = 0; i < list->len; ++i) {
		check[*(uint32_t*)(it->val)]++;
		it = it->nxt;
	}

	printf("# Expect list len: %d got %d\n",num_threads * 5, list->len);
	printf("# Expect 0's amount: %d got %d\n",num_threads,check[0]);
	printf("# Expect 1's amount: %d got %d\n",num_threads,check[1]);
	printf("# Expect 2's amount: %d got %d\n",num_threads,check[2]);
	printf("# Expect 3's amount: %d got %d\n",num_threads,check[3]);
	printf("# Expect 4's amount: %d got %d\n",num_threads,check[4]);

    free(threads);
    ll_delete(list);
    return 0;
}
