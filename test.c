#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
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

static bool contains_filename(FileList *list, FileName file) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (!strcmp(list->files[i].name, file.name)) 
            return true;
    }
    return false;
}

static void brn2_test(void **state) {
    (void) state;
    char *command = "ls -a > /tmp/brn2test";
    char *file = command + 8;
    system(command);
    FileList *list1 = brn2_list_from_dir(".");
    FileList *list2 = brn2_list_from_lines(file, 0);

    assert_int_equal(list1->length, list2->length);

    for (uint32 i = 0; i < list1->length; i += 1) {
        assert_true(contains_filename(list2, list1->files[i]));
    }

    brn2_free_list(list1);
    brn2_free_list(list2);
    unlink(file);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(hash_test),
        cmocka_unit_test(brn2_test),
    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
