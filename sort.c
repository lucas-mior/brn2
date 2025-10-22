/*
 * Copyright (C) 2025 Mior, Lucas;
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(SORT_C)
#define SORT_C

#include "brn2.h"
#include <string.h>
#include <stdlib.h>

#include "util.c"

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__ == 0
#define COMPARE(A, B) compare_func(A, B)
#elif defined(TESTING_sort)
#define COMPARE(A, B) compare_func(A, B)
#else
#define COMPARE(A, B) brn2_compare(A, B)
#endif

#if !defined(LENGTH)
#define LENGTH(x) (isize)((sizeof(x) / sizeof(*x)))
#endif

typedef struct HeapNode {
    void *value;
    uint32 p_index;
    uint32 unused;
} HeapNode;

static void
sort_shuffle(void *array, usize n, usize size) {
    char *tmp = xmalloc(size);
    char *arr = array;

    if (n > 1) {
        for (usize i = 0; i < n - 1; i += 1) {
            usize rnd = (usize)rand();
            usize j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j*size, size);
            memcpy(arr + j*size, arr + i*size, size);
            memcpy(arr + i*size, tmp, size);
        }
    }

    free(tmp);
    return;
}

static void
sort_heapify(HeapNode *heap, uint32 p, uint32 i,
             int32 (*compare_func)(const void *a, const void *b)) {
    (void)compare_func;
    while (true) {
        uint32 smallest = i;
        uint32 left = 2*i + 1;
        uint32 right = 2*i + 2;

        if (left >= p) {
            break;
        }

        if (COMPARE(heap[left].value, heap[smallest].value) < 0) {
            smallest = left;
        }
        if ((right < p)
            && COMPARE(heap[right].value, heap[smallest].value) < 0) {
            smallest = right;
        }

        if (smallest == i) {
            break;
        }

        {
            HeapNode temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            i = smallest;
        }
    }
    return;
}

static void
sort_merge_subsorted(void *array, uint32 n, uint32 p, usize size,
                     void *dummy_last,
                     int32 (*compare)(const void *a, const void *b)) {
    HeapNode heap[BRN2_MAX_THREADS];
    uint32 n_sub[BRN2_MAX_THREADS];
    uint32 indices[BRN2_MAX_THREADS] = {0};
    uint32 offsets[BRN2_MAX_THREADS];
    usize memory_size = size*n;
    char *output = xmalloc(memory_size);
    char *array2 = array;

    for (uint32 k = 0; k < (p - 1); k += 1) {
        n_sub[k] = n / p;
    }
    {
        uint32 k = p - 1;
        n_sub[k] = n / p + (n % p);
    }

    offsets[0] = 0;
    for (uint32 k = 1; k < p; k += 1) {
        offsets[k] = offsets[k - 1] + n_sub[k - 1];
    }

    for (uint32 k = 0; k < p; k += 1) {
        heap[k].value = xmalloc(size);
        memcpy(heap[k].value, &array2[offsets[k]*size], size);
        heap[k].p_index = k;
    }

    for (int32 k = p / 2 - 1; k >= 0; k -= 1) {
        sort_heapify(heap, p, (uint32)k, compare);
    }

    for (uint32 i = 0; i < n; i += 1) {
        uint32 k = heap[0].p_index;
        uint32 i_sub = (indices[k] += 1);

        memcpy(&output[i*size], heap[0].value, size);

        if (i_sub < n_sub[k]) {
            memcpy(heap[0].value, &array2[(offsets[k] + i_sub)*size], size);
        } else {
            memcpy(heap[0].value, dummy_last, size);
        }
        sort_heapify(heap, p, 0, compare);
    }

    memcpy(array2, output, n*size);
    free(output);
    for (uint32 i = 0; i < p; i += 1) {
        free(heap[i].value);
    }
    return;
}

#define SORT_BENCHMARK 0

#if defined(__INCLUDE_LEVEL__) && __INCLUDE_LEVEL__
static void
sort(FileList *old) {
    uint32 p;

    char *last = "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                 "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
                 "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
    uint32 last_length = (uint32)strlen(last);
    FileName *dummy_last;

    dummy_last = xmalloc(STRUCT_ARRAY_SIZE(dummy_last, char, last_length + 1));
    memset(dummy_last, 0, sizeof(*dummy_last));
    memcpy(dummy_last, last, last_length + 1);

#if SORT_BENCHMARK
#if !defined(__WIN32__)
#include "qsort_glibc.c"
#endif

    struct timespec t0;
    struct timespec t1;

    FileList copy = {0};
    sort_shuffle(old->files, old->length, sizeof(*(old->files)));

    memcpy(&copy, old, sizeof(*old));
    copy.files = xmalloc(copy.length*sizeof(*(old->files)));
    memcpy(copy.files, old->files, copy.length*sizeof(*(old->files)));
    clock_gettime(CLOCK_MONOTONIC_RAW, &t0);
#endif

    p = brn2_threads(brn2_threads_work_sort, old, NULL, NULL, 0);
    if (p == 1) {
        free(dummy_last);
        return;
    }

    /* QSORT(old->files, old->length, sizeof(*(old->files)),
     * brn2_compare); */
    sort_merge_subsorted(old->files, old->length, p, sizeof(*(old->files)),
                         &dummy_last, brn2_compare);

#if SORT_BENCHMARK
    clock_gettime(CLOCK_MONOTONIC_RAW, &t1);
    QSORT(copy.files, copy.length, sizeof(*(copy.files)), brn2_compare);
    if (memcmp(copy.files, old->files, copy.length*sizeof(*(copy.files)))) {
        error("Error in sorting.\n");
        for (uint32 i = 0; i < old->length; i += 1) {
            char *name1 = old->files[i]->name;
            char *name2 = copy.files[i]->name;
            if (strcmp(name1, name2)) {
                error("[%u] = %s != %s\n", i, name1, name2);
            }
        }
        fatal(EXIT_FAILURE);
    } else {
        error("Sorting successful.\n");
    }
    brn2_timings("sorting", t0, t1, old->length);
    exit(EXIT_SUCCESS);
#endif

    free(dummy_last);
    return;
}
#endif

#if TESTING_sort

#define MAXI 10000
static const uint32 possibleN[] = {31, 32, 33, 50};
static const uint32 possibleP[] = {1, 2, 3, 8};

static int32
compare_int(const void *a, const void *b) {
    const int32 *aa = a;
    const int32 *bb = b;
    return *aa - *bb;
}
static int32 dummy = INT32_MAX;

static void
test_sorting(uint32 n, uint32 p) {
    int32 *array = xmalloc(n*sizeof(*array));
    uint32 *n_sub = xmalloc(p*sizeof(*n_sub));

    if (n < p*2) {
        fprintf(stderr, "n=%u must be larger than p*2=%u*2\n", n, p);
        exit(EXIT_SUCCESS);
    }

    for (uint32 i = 0; i < (p - 1); i += 1) {
        n_sub[i] = n / p;
    }
    {
        uint32 i = p - 1;
        n_sub[i] = n / p + (n % p);
    }

    printf("n_sub[P - 1] = %u\n", n_sub[p - 1]);

    srand(42);
    for (uint32 i = 0; i < n; i += 1) {
        array[i] = rand() % MAXI;
    }

    {
        uint32 offset = 0;
        for (uint32 i = 0; i < p; i += 1) {
            qsort(&array[offset], n_sub[i], sizeof(*array), compare_int);
            offset += n_sub[i];
        }
    }

    sort_merge_subsorted(array, n, p, sizeof(int32), &dummy, compare_int);

    for (uint32 i = 0; i < n; i += 1) {
        if (i < (n - 1)) {
            assert(array[i] <= array[i + 1]);
        }
    }

    free(array);
    free(n_sub);
    return;
}

int
main(void) {
    for (uint32 in = 0; in < LENGTH(possibleN); in += 1) {
        for (uint32 ip = 0; ip < LENGTH(possibleP); ip += 1) {
            test_sorting(possibleN[in], possibleP[ip]);
        }
    }
    exit(EXIT_SUCCESS);
}

#endif

#endif
