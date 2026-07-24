#if !defined(CBASE_SORT_H)
#define CBASE_SORT_H

#if !defined(SORT_COMPARE)
#define SORT_COMPARE(A, B) compare_func(A, B)
#endif

#if !defined(MAX_NTHREADS)
#define MAX_NTHREADS 64
#endif

typedef struct HeapNode {
    void *value;
    int32 p_index;
    int32 unused;
} HeapNode;

static void sort_functions_sink(void);
static void sort_heapify(HeapNode *, int32, int32, int32 (*)(void *, void *));
static void sort_merge_subsorted(
    void *,
    int32,
    int32,
    int64,
    int32 (*)(void *, void *)
);
static void sort_shuffle(void *, int64, int64);

#endif /* CBASE_SORT_H */
