/* Copyright (C) 1991-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#if !defined(QSORT_GLIBC_C)
#define QSORT_GLIBC_C

#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#if !defined(COMPARE)
#define COMPARE compare_func
#endif

typedef int (*compar_d_fn_t2)(const void *, const void *);
static void qsort_glibc(void *const, size_t, size_t, compar_d_fn_t2);

#define min(x, y) ((x) < (y) ? (x) : (y))

/* Byte-wise swap two items of size SIZE. */
#define SWAP_BYTES(a, b, size)                                                 \
    do {                                                                       \
        size_t SWAPsize = (size);                                              \
        char *SWAPa = (a);                                                     \
        char *SWAPb = (b);                                                     \
        do {                                                                   \
            char SWAPtmp = *SWAPa;                                             \
            *SWAPa++ = *SWAPb;                                                 \
            *SWAPb++ = SWAPtmp;                                                \
        } while (--SWAPsize > 0);                                              \
    } while (0)

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct StackNode {
    char *lo;
    char *hi;
} StackNode;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type size_t, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT)*sizeof(size_t).  */
#define STACK_SIZE (CHAR_BIT*sizeof(size_t))
#define PUSH(low, high) ((void)((top->lo = (low)), (top->hi = (high)), ++top))
#define POP(low, high) ((void)(--top, (low = top->lo), (high = top->hi)))

/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32*sizeof(StackNode) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.

   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.

   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.

   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */

#include <stdlib.h>

void
qsort_glibc(void *const pbase, size_t total_elems, size_t size,
            compar_d_fn_t2 compare_func) {
    char *base_ptr = pbase;
    const size_t max_thresh = MAX_THRESH*size;
    (void)compare_func;

    if (total_elems == 0) {
        /* Avoid lossage with unsigned arithmetic below.  */
        return;
    }

    if (total_elems > MAX_THRESH) {
        char *lo = base_ptr;
        char *hi = &lo[size*(total_elems - 1)];
        StackNode stack[STACK_SIZE];
        StackNode *top = stack;

        PUSH(NULL, NULL);

        while (stack < top) {
            char *left;
            char *right;

            /* Select median value from among LO, MID, and HI. Rearrange
             * LO and HI so the three values are sorted. This lowers the
             * probability of picking a pathological pivot value and
             * skips a comparison for both the left and right in
             * the while loops. */

            char *mid = lo + size*((size_t)(hi - lo) / size >> 1);

            do {
                if (COMPARE((void *)mid, (void *)lo) < 0) {
                    SWAP_BYTES(mid, lo, size);
                }

                if (COMPARE((void *)hi, (void *)mid) < 0) {
                    SWAP_BYTES(mid, hi, size);
                } else {
                    break;
                }

                if (COMPARE((void *)mid, (void *)lo) < 0) {
                    SWAP_BYTES(mid, lo, size);
                }
            } while (0);

            left = lo + size;
            right = hi - size;

            /* Here's the famous ``collapse the walls'' section of quicksort.
             * Gotta like those tight inner loops!  They are the main reason
             * that this algorithm runs much faster than others. */
            do {
                while (COMPARE((void *)left, (void *)mid) < 0) {
                    left += size;
                }

                while (COMPARE((void *)mid, (void *)right) < 0) {
                    right -= size;
                }

                if (left < right) {
                    SWAP_BYTES(left, right, size);

                    if (mid == left) {
                        mid = right;
                    } else if (mid == right) {
                        mid = left;
                    }

                    left += size;
                    right -= size;
                } else if (left == right) {
                    left += size;
                    right -= size;
                    break;
                }
            } while (left <= right);

            /* Set up pointers for next iteration.  First determine whether
             * left and right partitions are below the threshold size.  If so,
             * ignore one or both.  Otherwise, push the larger partition's
             * bounds on the stack and continue sorting the smaller one. */

            if ((size_t)(right - lo) <= max_thresh) {
                if ((size_t)(hi - left) <= max_thresh) {
                    /* Ignore both small partitions. */
                    POP(lo, hi);
                } else {
                    /* Ignore small left partition. */
                    lo = left;
                }
            } else if ((size_t)(hi - left) <= max_thresh) {
                /* Ignore small right partition. */
                hi = right;
            } else if ((right - lo) > (hi - left)) {
                /* Push larger left partition indices. */
                PUSH(lo, right);
                lo = left;
            } else {
                /* Push larger right partition indices. */
                PUSH(left, hi);
                hi = right;
            }
        }
    }

    /* Once the BASE_PTR array is partially sorted by quicksort the rest
     * is completely sorted using insertion sort, since this is efficient
     * for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     * of the array to sort, and END_PTR points at the very last element in
     * the array (*not* one beyond it!). */
    {
        char *const end_ptr = &base_ptr[size*(total_elems - 1)];
        char *tmp_ptr = base_ptr;
        char *thresh = min(end_ptr, base_ptr + max_thresh);
        char *run_ptr;

        /* Find smallest element in first threshold and place it at the
         * array's beginning.  This is the smallest array element,
         * and the operation speeds up insertion sort's inner loop. */

        for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size) {
            if (COMPARE((void *)run_ptr, (void *)tmp_ptr) < 0) {
                tmp_ptr = run_ptr;
            }
        }

        if (tmp_ptr != base_ptr) {
            SWAP_BYTES(tmp_ptr, base_ptr, size);
        }

        /* Insertion sort, running from left-hand-side up to right-hand-side. */

        run_ptr = base_ptr + size;
        while ((run_ptr += size) <= end_ptr) {
            tmp_ptr = run_ptr - size;
            while (COMPARE((void *)run_ptr, (void *)tmp_ptr) < 0) {
                tmp_ptr -= size;
            }

            tmp_ptr += size;
            if (tmp_ptr != run_ptr) {
                char *trav;

                trav = run_ptr + size;
                while (--trav >= run_ptr) {
                    char c = *trav;
                    char *hi, *lo;

                    for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo) {
                        *hi = *lo;
                    }
                    *hi = c;
                }
            }
        }
    }
    return;
}

#if defined(TESTING_qsort_glibc)

#include "brn2.h"
#include "util.c"

#define MAXI 10000

static int32
compare_int(const void *a, const void *b) {
    const int32 *aa = a;
    const int32 *bb = b;
    return *aa - *bb;
}

int
main(void) {
    int32 n = 100;
    int32 *array = xmalloc((size_t)n*sizeof(*array));

    srand(42);
    for (int32 i = 0; i < n; i += 1) {
        array[i] = rand() % MAXI;
    }

    qsort_glibc(array, (size_t)n, sizeof(*array), compare_int);

    for (int32 i = 0; i < (n - 1); i += 1) {
        assert(array[i] <= array[i + 1]);
    }

    free(array);
    exit(EXIT_SUCCESS);
}

#endif

#endif
