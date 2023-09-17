#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "brn2.h"
#include "hash.h"
#include "util.h"

static void hash_test(void **state) {
    (void) state;
    HashSet *set = hash_set_create(1000);
    assert_non_null(set);
    assert_true(hash_set_capacity(set) >= 1000);
    assert_true(hash_set_insert(set, "a", 1));
    assert_false(hash_set_insert(set, "a", 1));
    assert_true(hash_set_insert(set, "b", 1));
    assert_true(hash_set_length(set) == 2);
    assert_false(hash_set_remove(set, "c", 1));
    assert_true(hash_set_remove(set, "b", 1));
    assert_true(hash_set_length(set) == 1);
    hash_set_destroy(set);
}

static void brn2_test(void **state) {
    (void) state;
    FileList *list = brn2_list_from_dir(".");
    assert_non_null(list);
    brn2_free_list(list);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(hash_test),
        cmocka_unit_test(brn2_test),
    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
