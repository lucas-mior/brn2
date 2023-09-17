#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(hash_test),
    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
