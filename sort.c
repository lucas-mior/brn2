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

#ifndef SORT_C
#define SORT_C

#include "brn2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef TESTING_THIS_FILE
  #define TESTING_THIS_FILE 0
#endif

#pragma push_macro("TESTING_THIS_FILE")
#define TESTING_THIS_FILE 0

#include "util.c"

#pragma pop_macro("TESTING_THIS_FILE")

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
            usize rnd = (usize) rand();
            usize j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j*size, size);
            memcpy(arr + j*size, arr + i*size, size);
            memcpy(arr + i*size, tmp, size);
        }
    }

    free(tmp);
}

static void
sort_heapify(HeapNode *heap, uint32 p, uint32 i,
        int32 (*compare)(const void *a, const void *b)) {
    while (true) {
        uint32 smallest = i;
        uint32 left = 2*i + 1;
        uint32 right = 2*i + 2;

        if (left >= p)
            break;

        if (compare(heap[left].value, heap[smallest].value) < 0)
            smallest = left;
        if ((right < p) && compare(heap[right].value, heap[smallest].value) < 0)
            smallest = right;

        if (smallest == i)
            break;

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
        n_sub[k] = n/p;
    }{
        uint32 k = p - 1;
        n_sub[k] = n/p + (n % p);
    }

    offsets[0] = 0;
    for (uint32 k = 1; k < p; k += 1)
        offsets[k] = offsets[k - 1] + n_sub[k - 1];

    for (uint32 k = 0; k < p; k += 1) {
        heap[k].value = xmalloc(size);
        memcpy(heap[k].value, &array2[offsets[k]*size], size);
        heap[k].p_index = k;
    }

    for (int32 k = p / 2 - 1; k >= 0; k -= 1)
        sort_heapify(heap, p, (uint32)k, compare);

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
    for (uint32 i = 0; i < p; i += 1)
        free(heap[i].value);
    return;
}

#define SORT_BENCHMARK 0

#if !TESTING_THIS_FILE
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

    p = brn2_threads(brn2_threads_work_sort, old, NULL, NULL, NULL, 0);
    if (p == 1)
        return;

    /* qsort(old->files, old->length, sizeof(*(old->files)), brn2_compare); */
    sort_merge_subsorted(old->files, old->length, p,
                           sizeof(*(old->files)),
                           &dummy_last, brn2_compare);
    return;
}
#endif

#if TESTING_THIS_FILE

#define P 16u
#define MAXI 10000
static const uint32 possibleN[] = {100};

static int32 compare_count = 0;
static int32
compare_int(const void *a, const void *b) {
    const int32 *aa = a;
    const int32 *bb = b;
    compare_count += 1;
    return *aa - *bb;
}
static int32 dummy = INT32_MAX;

#define LENGTH(X) (uint32)(sizeof(X) / sizeof(*X))

int
main(void) {
    for (uint32 in = 0; in < LENGTH(possibleN); in += 1) {
        const uint32 n = possibleN[in];
        int32 *array = xmalloc(n*sizeof(*array));
        uint32 n_sub[P];
        const uint32 p = P;

        if (n < P*2) {
            fprintf(stderr, "n=%u must be larger than P*2=%u*2\n", n, P);
            exit(EXIT_SUCCESS);
        }

        for (uint32 i = 0; i < (p - 1); i += 1) {
            n_sub[i] = n/p;
        }{
            uint32 i = p - 1;
            n_sub[i] = n/p + (n % p);
        }

        printf("n_sub[P-1] = %u\n", n_sub[p-1]);

        srand(42);
        for (uint32 i = 0; i < n; i += 1) {
            array[i] = rand() % MAXI;
            if (i < 3 || (n - i) < 3)
                printf("array[%u] = %d\n", i, array[i]);
        }

        {
            uint32 offset = 0;
            for (uint32 i = 0; i < p; i += 1) {
                qsort(&array[offset], n_sub[i], sizeof(*array), compare_int);
                offset += n_sub[i];
            }
        }

        sort_merge_subsorted(array, n, p, sizeof(int32), &dummy, compare_int);

        {
            uint32 digits_i = 1;
            uint32 n2 = n;
            int digits_a = 1;
            int max = MAXI;

            while (n2 > 10) {
                n2 /= 10;
                digits_i += 1;
            }
            while (max > 10) {
                max /= 10;
                digits_a += 1;
            }

            for (uint32 i = 0; i < n; i += 1) {
                if (i % 10 == 0)
                    printf("\n[%.*u] ", digits_i, i);
                printf("%.*d ", digits_a, array[i]);

                if (i < (n-1)) {
                    assert(array[i] <= array[i + 1]);
                }
            }
            printf("\n");
        }
    }
}

#endif

#endif
