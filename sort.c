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
shuffle(void *array, usize n, usize size) {
    char *tmp = xmalloc(size);
    char *arr = array;
    usize stride = size*sizeof(char);

    if (n > 1) {
        usize i;
        for (i = 0; i < n - 1; ++i) {
            usize rnd = (usize) rand();
            usize j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j*stride, size);
            memcpy(arr + j*stride, arr + i*stride, size);
            memcpy(arr + i*stride, tmp, size);
        }
    }
}

typedef struct HeapNode {
    void *value;
    uint32 array_index;
    uint32 element_index;
} HeapNode;

static void
swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static void
heapify(HeapNode *heap, uint32 n, uint32 i,
        int32 (*compare)(const void *a, const void *b)) {
    uint32 smallest = i;
    uint32 left = 2*i + 1;
    uint32 right = 2*i + 2;

    if ((left < n) && compare(heap[left].value, heap[smallest].value) < 0)
        smallest = left;
    if ((right < n) && compare(heap[right].value, heap[smallest].value) < 0)
        smallest = right;
    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        heapify(heap, n, smallest, compare);
    }
    return;
}

static void
merge_sorted_subarrays(void *array, uint32 n, uint32 p, usize size, void *dummy,
                       int32 (*compare)(const void *a, const void *b)) {
    HeapNode heap[BRN2_MAX_THREADS];
    uint32 nsub[BRN2_MAX_THREADS];
    uint32 indices[BRN2_MAX_THREADS] = {0};
    uint32 offsets[BRN2_MAX_THREADS];
    usize memory_size = size*n;
    char *output = xmmap(&memory_size);
    char *array2 = array;

    for (uint32 i = 0; i < (p - 1); i += 1) {
        nsub[i] = n/p;
    }{
        uint32 i = p - 1;
        nsub[i] = n/p + (n % p);
    }

    offsets[0] = 0;
    for (uint32 i = 1; i < p; i++)
        offsets[i] = offsets[i - 1] + nsub[i - 1];

    for (uint32 i = 0; i < p; i++) {
        heap[i].value = xmalloc(size);
        memcpy(heap[i].value, &array2[offsets[i]*size], size);
        heap[i].array_index = i;
        heap[i].element_index = 0;
    }

    for (int32 i = p / 2 - 1; i >= 0; i--)
        heapify(heap, p, (uint32)i, compare);

    for (uint32 i = 0; i < n; i++) {
        uint32 arr_idx = heap[0].array_index;
        uint32 elem_idx = ++indices[arr_idx];
        memcpy(&output[i*size], heap[0].value, size);

        if (elem_idx < nsub[arr_idx]) {
            memcpy(heap[0].value, &array2[(offsets[arr_idx] + elem_idx)*size], size);
            heap[0].element_index = elem_idx;
        } else {
            memcpy(heap[0].value, dummy, size);
        }

        heapify(heap, p, 0, compare);
    }

    memcpy(array2, output, n*size);
    xmunmap(output, memory_size);
    for (uint32 i = 0; i < p; i += 1)
        free(heap[i].value);
    return;
}

#define SORT_CHECK 0

static void
sort(FileList *old) {
    struct timespec t0;
    struct timespec t1;
    uint32 p;
    FileName dummy = {
        .name = "\077\077",
        .hash = 0,
        .length = 0,
        .type = 0,
        .unused = 0,
    };

#if SORT_CHECK
    usize lsize = STRUCT_ARRAY_SIZE(old, FileName, old->length);
    FileList *copy = xmalloc(lsize);

    shuffle(old->files, old->length, sizeof(*(old->files)));
    memcpy(copy, old, lsize);
#endif

    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
    p = brn2_threads(brn2_threads_work_sort, old, NULL, NULL, NULL, 0);

    if (p == 1)
        return;

    /* qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare); */
    merge_sorted_subarrays(old->files, old->length, p,
                           sizeof(*(old->files)),
                           &dummy, brn2_compare);

#if SORT_CHECK
    qsort(copy->files, copy->length, sizeof(*(copy->files)), brn2_compare);
    if (memcmp(copy, old, lsize)) {
        error("copy is different than old!\n");
        for (int32 i = 0; i < old->length; i += 1) {
            error("[%u] = %s != %s\n",
                  i, old->files[i].name, copy->files[i].name);
        }
    } else {
        error("copy is EQUAL TO old!\n");
    }
    free(copy);
#endif

    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);

    {
        long seconds = t1.tv_sec - t0.tv_sec;
        long nanos = t1.tv_nsec - t0.tv_nsec;

        double total_seconds = (double)seconds + (double)nanos/1.0e9;
        double micros_per_str = 1e6*(total_seconds/(double)(old->length));

        printf("\ntime elapsed %s:%s\n", __FILE__, "sort");
        printf("%gs = %gus per string.\n\n", total_seconds, micros_per_str);
        /* exit(0); */
    }
    return;
}

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE

#define N 200
#define P 16
#define MAXI 1000

int32
compare(const void *a, const void *b) {
    return (*(int32*)a - *(int32*)b);
}

uint32 possibleN[] = {32, 33, 34, 35, 100, 200};
#define LENGTH(X) (uint32)(sizeof(X) / sizeof(*X))

int main(void) {
    int32 array[N];
    uint32 nsub[P];
    if (N < P*2) {
        fprintf(stderr, "N=%d must be larger than P*2=%d*2\n", N, P);
        exit(EXIT_SUCCESS);
    }
    const uint32 n = N;
    const uint32 p = P;
    bool isvalidN = false;

    for (uint32 i = 0; i < LENGTH(possibleN); i += 1) {
        if (possibleN[i] == n) {
            isvalidN = true;
            break;
        }
    }
    if (!isvalidN) {
        error("Invalid N. Must be one of:\n");
        for (uint32 i = 0; i < LENGTH(possibleN); i += 1) {
            error("%d ", possibleN[i]);
        }
        exit(EXIT_FAILURE);
    }

    for (uint32 i = 0; i < (p - 1); i += 1) {
        nsub[i] = n/p;
    }{
        uint32 i = p - 1;
        nsub[i] = n/p + (n % p);
    }

    printf("nsub[P-1] = %d\n", nsub[p-1]);

    srand(42);
    for (uint32 i = 0; i < n; i++) {
        array[i] = rand() % MAXI;
    }

    {
        uint32 offset = 0;
        for (uint32 i = 0; i < p; i += 1) {
            qsort(&array[offset], nsub[i], sizeof(*array), compare);
            offset += nsub[i];
        }
    }

    {
        uint32 index = 0;
        for (uint32 i = 0; i < p; i++) {
            printf("nsub[%d] = %d\n", i, nsub[i]);
            for (uint32 j = 0; j < nsub[i]; j++, index++) {
                printf("array[%d]: %d\n", index, array[index]);
            }
            printf("\n");
        }
    }

    int32 dummy = INT_MAX;

    merge_sorted_subarrays(array, n, p, sizeof(int32), &dummy, compare);

    for (uint32 i = 0; i < n; i++) {
        printf("%u ", array[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    switch (N) {
        case 32:
        case 33:
        case 34:
        case 35:
            assert(array[0] == 12);
            assert(array[n-1] == 940);
            break;
        case 100:
            assert(array[0] == 12);
            assert(array[n-1] == 995);
            break;
        case 200:
            assert(array[0] == 7);
            assert(array[n-1] == 995);
            break;
        default:
            error("Invalid N=%u value.\n", n);
            exit(EXIT_FAILURE);
    }
}

#endif

#endif
