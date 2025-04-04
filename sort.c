#ifndef SORT_C
#define SORT_C

#include "brn2.h"
#include <string.h>

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

#endif
