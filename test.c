/*
 * Copyright (C) 2023 Mior, Lucas; <lucasbuddemior@gmail.com>
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

#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "brn2.h"
#include "hash.h"
#include "util.h"

static void hash_test(void **state) {
    HashMap *set = hash_map_create(1000);
    assert_non_null(set);
    assert_true(hash_map_capacity(set) >= 1000);

    assert_true(hash_map_insert(set, "a", 0));
    assert_false(hash_map_insert(set, "a", 1));
    assert_true(hash_map_insert(set, "b", 2));

    assert_true(hash_map_length(set) == 2);
    assert_true(*hash_map_lookup(set, "a") == 0);
    assert_null(hash_map_lookup(set, "c"));

    assert_false(hash_map_remove(set, "c"));
    assert_true(hash_map_remove(set, "b"));

    assert_true(hash_map_length(set) == 1);
    hash_map_destroy(set);
    (void) state;
    return;
}

static bool contains_filename(FileList *list, FileName file) {
    for (uint32 i = 0; i < list->length; i += 1) {
        if (!strcmp(list->files[i].name, file.name))
            return true;
    }
    return false;
}

static void brn2_test(void **state) {
    FileList *list1;
    FileList *list2;
    char *command = "ls -a > /tmp/brn2test";
    char *file = command + 8;
    system(command);
    list1 = brn2_list_from_dir(".");
    list2 = brn2_list_from_lines(file, 0);

    assert_true(list1->length == list2->length);

    for (uint32 i = 0; i < list1->length; i += 1)
        assert_true(contains_filename(list2, list1->files[i]));

    brn2_free_list(list1);
    brn2_free_list(list2);
    unlink(file);
    (void) state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(hash_test),
        cmocka_unit_test(brn2_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
