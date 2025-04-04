#ifndef SORT_C
#define SORT_C

#include "brn2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void
shuffle(void *array, size_t n, size_t size) {
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}

void
mergesort(void *base, size_t nitems, size_t size,
          int (*compar)(const void *, const void *)) {
    return;
}

void
sort(FileList *old) {
    struct timespec t0;
    struct timespec t1;

    usize lsize = STRUCT_ARRAY_SIZE(old, FileName, old->length);
    FileList *copy = xmalloc(lsize);

    shuffle(old->files, old->length, sizeof(*(old->files)));
    memcpy(copy, old, lsize);

    brn2_threads(brn2_threads_work_sort, old, NULL, NULL, NULL, 0);
    qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);

    mergesort(copy->files, copy->length, sizeof(*(copy->files)), brn2_compare);
    qsort(copy->files, copy->length, sizeof(*(copy->files)), brn2_compare);

    if (memcmp(copy, old, lsize)) {
        error("copy is different than old!\n");
    } else {
        error("copy is EQUAL TO old!\n");
    }
    /* for (uint32 i = 0; i < old->length; i += 1) { */
    /*     printf("[%u] = %s\n", i, old->files[i].name); */
    /* } */

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);

    {
        long seconds = t1.tv_sec - t0.tv_sec;
        long nanos = t1.tv_nsec - t0.tv_nsec;

        double total_seconds = (double)seconds + (double)nanos/1.0e9;
        double micros_per_str = 1e6*(total_seconds/(double)(old->length));

        printf("\ntime elapsed %s:%s\n", __FILE__, "sort");
        printf("%gs = %gus per string.\n\n", total_seconds, micros_per_str);
        exit(0);
    }
}

typedef struct {
    int value;
    int array_index;
    int element_index;
} HeapNode;

void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(HeapNode *heap, int n, int i,
             int (*compare)(const void *a, const void *b)) {
    int smallest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && compare(&heap[left].value, &heap[smallest].value) < 0)
        smallest = left;
    if (right < n && compare(&heap[right].value, &heap[smallest].value) < 0)
        smallest = right;
    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        heapify(heap, n, smallest, compare);
    }
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void merge_sorted_subarrays(int array[], int n, int nsub,
                            int (*compare)(const void *a, const void *b)) {
    int output[n];
    HeapNode heap[4];

    int indices[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        heap[i].value = array[i*nsub];
        heap[i].array_index = i;
        heap[i].element_index = 0;
    }

    for (int i = 4 / 2 - 1; i >= 0; i--) {
        heapify(heap, 4, i, compare);
    }

    for (int i = 0; i < n; i++) {
        output[i] = heap[0].value;
        int arr_idx = heap[0].array_index;
        int elem_idx = ++indices[arr_idx];

        if (elem_idx < nsub) {
            heap[0].value = array[arr_idx*nsub + elem_idx];
            heap[0].element_index = elem_idx;
        } else {
            heap[0].value = __INT_MAX__;
        }

        heapify(heap, 4, 0, compare);
    }

    for (int i = 0; i < n; i++) {
        array[i] = output[i];
    }
}

#if TESTING_THIS_FILE

#define SIZE 100
#define SUB_SIZE (SIZE / 4)

int main(void) {
    int array[SIZE];

    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % 1000;
    }

    for (int i = 0; i < SIZE; i += SUB_SIZE) {
        qsort(&array[i], SUB_SIZE, sizeof(int), compare);
    }

    merge_sorted_subarrays(array, SIZE, SUB_SIZE, compare);

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", array[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    return 0;
}

#endif

#endif
