#ifndef SORT_C
#define SORT_C

#include "brn2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "util.c"

#pragma pop_macro("TESTING_THIS_FILE")

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

typedef struct {
    void *value;
    int array_index;
    int element_index;
} HeapNode;

static void
swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static void
heapify(HeapNode *heap, int n, int i,
        int (*compare)(const void *a, const void *b)) {
    int smallest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && compare(heap[left].value, heap[smallest].value) < 0)
        smallest = left;
    if (right < n && compare(heap[right].value, heap[smallest].value) < 0)
        smallest = right;
    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        heapify(heap, n, smallest, compare);
    }
}

static void
merge_sorted_subarrays(void *array, int n, int p, usize size, void *dummy,
                       int (*compare)(const void *a, const void *b)) {
    char *output = xmalloc(size*n);
    HeapNode heap[p];
    char *array2 = array;
    int nsub = n / p;

    int indices[p];
    memset(indices, 0, p*sizeof(*indices));


    for (int i = 0; i < p; i++) {
        heap[i].value = xmalloc(size);
        memcpy(heap[i].value, &array2[i*nsub*size], size);
        heap[i].array_index = i;
        heap[i].element_index = 0;
    }

    for (int i = p / 2 - 1; i >= 0; i--) {
        heapify(heap, p, i, compare);
    }

    for (int i = 0; i < n; i++) {
        memcpy(&output[i*size], heap[0].value, size);
        int arr_idx = heap[0].array_index;
        int elem_idx = ++indices[arr_idx];

        if (elem_idx < nsub) {
            memcpy(heap[0].value, &array2[(arr_idx*nsub + elem_idx)*size], size);
            heap[0].element_index = elem_idx;
        } else {
            memcpy(heap[0].value, dummy, size);
        }

        heapify(heap, p, 0, compare);
    }

    memcpy(array2, output, n*size);
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

    int nthreads;
    nthreads = brn2_threads(brn2_threads_work_sort, old, NULL, NULL, NULL, 0);
    qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    FileName dummy = {
        .name = "\077\077",
        .hash = 0,
        .length = 0,
        .type = 0,
        .unused = 0,
    };

    merge_sorted_subarrays(old->files, old->length, nthreads,
                           sizeof(*(old->files)),
                           &dummy, brn2_compare);

    qsort(copy->files, copy->length, sizeof(*(copy->files)), brn2_compare);

    if (memcmp(copy, old, lsize)) {
        error("copy is different than old!\n");
    } else {
        error("copy is EQUAL TO old!\n");
    }
    for (uint32 i = 0; i < old->length; i += 1) {
        printf("[%u] = %s != %s\n", i, old->files[i].name, copy->files[i].name);
    }

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

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE

#define N 20
#define P 4

int
compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(void) {
    int array[N];
    int nsub = N/P;

    for (int i = 0; i < N; i++) {
        array[i] = rand() % 1000;
    }

    for (int i = 0; i < N; i += nsub) {
        qsort(&array[i], nsub, sizeof(int), compare);
    }

    int dummy = INT_MAX;

    merge_sorted_subarrays(array, N, P, sizeof(int), &dummy, compare);

    for (int i = 0; i < N; i++) {
        printf("%d ", array[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    return 0;
}

#endif

#endif
