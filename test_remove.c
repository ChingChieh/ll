#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "ll.h"

#define DEFAULT_THREAD_NUM 1
#define DEFAULT_NODE_NUM 10000
ll_t *list;
void *test(void *ptr) {
	if(*(int*)ptr > DEFAULT_NODE_NUM){
		return NULL;
	}
	int it = DEFAULT_NODE_NUM / *(int*)ptr;
	for (int i = 0; i < it; ++i) {
		//ll_print(*list);
		ll_remove_n(list,0);
	}
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
	uint32_t node_num = DEFAULT_NODE_NUM;
    // Start to create threads 
    pthread_t *threads;
    if ((threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t))) == NULL){
        perror("malloc");
        exit(1);
    }
	// Testing array & empty array use for checking correctness
	uint32_t num_array[5] = {0,1,2,3,4};
	// Create a 10000 nodes list
	for (int i = 0; i < node_num; ++i) {
		ll_insert_last(list,&num_array[i % 5]);	
	}

    for (int i = 0; i < num_threads; ++i) {
		if(pthread_create(&threads[i], NULL, test, &num_threads) != 0){
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
    free(threads);
    ll_delete(list);
    return 0;
}
