#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 4


int *array;
int total_sum = 0;
pthread_mutex_t sum_mutex;


int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}


void *thread_sum(void *arg) {
    long thread_id = (long)arg;
    int start = (thread_id * (array_length / NUM_THREADS));
    int end = ((thread_id + 1) * (array_length / NUM_THREADS));
    int local_sum = 0;


    if (thread_id == NUM_THREADS - 1) {
        end = array_length;
    }

    for (int i = start; i < end; i++) {
        local_sum += array[i];
    }

    
    pthread_mutex_lock(&sum_mutex);
    total_sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}

int main() {
    int array_length;
    printf("Enter the size of array (N >= 16): ");
    scanf("%d", &array_length);

    if (array_length < 16) {
        printf("Error: N must be at least 16.\n");
        return 1;
    }

    
    srand(time(NULL));

    
    array = (int *)malloc(array_length * sizeof(int));
    if (array == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    
    for (int i = 0; i < array_length; i++) {
        array[i] = rand_int(-100, 100); 
    }

    
    pthread_mutex_init(&sum_mutex, NULL);

    
    pthread_t threads[NUM_THREADS];
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_sum, (void *)i);
    }

    
    printf("Array content:\n");
    for (int i = 0; i < array_length; i++) {
        printf("%d ", array[i]);
        if ((i + 1) % 10 == 0) { 
            printf("\n");
        }
    }
    printf("\n");

    
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    
    printf("Total sum: %d\n", total_sum);

    
    pthread_mutex_destroy(&sum_mutex);
    free(array);

    return 0;
}
