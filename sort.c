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

#define SORT_BENCHMARK 1

static void
shuffle(void *array, usize n, usize size) {
    char *tmp = xmalloc(size);
    char *arr = array;
    usize stride = size*sizeof(char);

    if (n > 1) {
        usize i;
        for (i = 0; i < n - 1; i += 1) {
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
merge_sorted_subarrays(void *array, uint32 n, uint32 p, usize size,
                       void *dummy_last,
                       int32 (*compare)(const void *a, const void *b)) {
    HeapNode heap[BRN2_MAX_THREADS];
    uint32 nsub[BRN2_MAX_THREADS];
    uint32 indices[BRN2_MAX_THREADS] = {0};
    uint32 offsets[BRN2_MAX_THREADS];
    usize memory_size = size*n;
    char *output = xmalloc(memory_size);
    char *array2 = array;

    for (uint32 i = 0; i < (p - 1); i += 1) {
        nsub[i] = n/p;
    }{
        uint32 i = p - 1;
        nsub[i] = n/p + (n % p);
    }

    offsets[0] = 0;
    for (uint32 i = 1; i < p; i += 1)
        offsets[i] = offsets[i - 1] + nsub[i - 1];

    for (uint32 i = 0; i < p; i += 1) {
        heap[i].value = xmalloc(size);
        memcpy(heap[i].value, &array2[offsets[i]*size], size);
        heap[i].array_index = i;
        heap[i].element_index = 0;
    }

    for (int32 i = p / 2 - 1; i >= 0; i -= 1)
        heapify(heap, p, (uint32)i, compare);

    for (uint32 i = 0; i < n; i += 1) {
        uint32 arr_idx = heap[0].array_index;
        uint32 elem_idx = ++indices[arr_idx];
        memcpy(&output[i*size], heap[0].value, size);

        if (elem_idx < nsub[arr_idx]) {
            memcpy(heap[0].value, &array2[(offsets[arr_idx] + elem_idx)*size], size);
            heap[0].element_index = elem_idx;
        } else {
            memcpy(heap[0].value, dummy_last, size);
        }

        heapify(heap, p, 0, compare);
    }

#if SORT_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif
    memcpy(array2, output, n*size);
    free(output);
#if SORT_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    brn2_timings("memcpy array2, output", t0, t1, n);
#endif
    for (uint32 i = 0; i < p; i += 1)
        free(heap[i].value);
    return;
}

static void
sort(FileList *old) {
    uint32 p;
    FileName dummy_last = {
        .name = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
        .hash = 0,
        .length = 0,
        .type = 0,
        .unused = 0,
    };

#if SORT_BENCHMARK
    struct timespec t0;
    struct timespec t1;
    usize list_size = STRUCT_ARRAY_SIZE(old, FileName, old->length);
    FileList *copy = xmalloc(list_size);

    shuffle(old->files, old->length, sizeof(*(old->files)));
    memcpy(copy, old, list_size);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif

    p = brn2_threads(brn2_threads_work_sort, old, NULL, NULL, NULL, 0);

    if (p == 1)
        return;

    /* qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare); */
    merge_sorted_subarrays(old->files, old->length, p,
                           sizeof(*(old->files)),
                           &dummy_last, brn2_compare);

#if SORT_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    qsort(copy->files, copy->length, sizeof(*(copy->files)), brn2_compare);
    if (memcmp(copy, old, list_size)) {
        error("copy is different than old!\n");
        for (uint32 i = 0; i < old->length; i += 1) {
            char *name1 = old->files[i].name;
            char *name2 = copy->files[i].name;
            if (strcmp(name1, name2)) {
                error("[%u] = %s != %s\n", i, name1, name2);
            }
        }
    } else {
        error("copy is EQUAL TO old!\n");
    }
    free(copy);
    brn2_timings("sorting", t0, t1, old->length);
    exit(0);
#endif
    return;
}

#ifndef TESTING_THIS_FILE
#define TESTING_THIS_FILE 0
#endif

#if TESTING_THIS_FILE

#define P 16u
#define MAXI 10000

static int32
compare_int(const void *a, const void *b) {
    const int32 *aa = a;
    const int32 *bb = b;
    return *aa - *bb;
}
static int32 dummy = INT32_MAX;

static const uint32 possibleN[] = {32, 33, 34, 35, 100};
#define LENGTH(X) (uint32)(sizeof(X) / sizeof(*X))

int
main(void) {
    for (uint32 in = 0; in < LENGTH(possibleN); in += 1) {
        const uint32 n = possibleN[in];
        int32 *array = xmalloc(n*sizeof(*array));
        uint32 nsub[P];
        const uint32 p = P;

        if (n < P*2) {
            fprintf(stderr, "n=%u must be larger than P*2=%u*2\n", n, P);
            exit(EXIT_SUCCESS);
        }

        for (uint32 i = 0; i < (p - 1); i += 1) {
            nsub[i] = n/p;
        }{
            uint32 i = p - 1;
            nsub[i] = n/p + (n % p);
        }

        printf("nsub[P-1] = %u\n", nsub[p-1]);

        srand(42);
        for (uint32 i = 0; i < n; i += 1) {
            array[i] = rand() % MAXI;
            if (i < 3 || (n - i) < 3)
                printf("array[%u] = %d\n", i, array[i]);
        }

        {
            uint32 offset = 0;
            for (uint32 i = 0; i < p; i += 1) {
                qsort(&array[offset], nsub[i], sizeof(*array), compare_int);
                offset += nsub[i];
            }
        }

        merge_sorted_subarrays(array, n, p, sizeof(int32), &dummy, compare_int);

        for (uint32 i = 0; i < n; i += 1) {
            printf("%d ", array[i]);
            if ((i+1) % 10 == 0)
                printf("\n");

            if (i < (n-1)) {
                assert(array[i] <= array[i + 1]);
            }
        }
        printf("\n");
    }
}

#endif

#endif
