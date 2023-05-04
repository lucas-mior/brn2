#include "brn2.h"

size_t hash_function(char *str) {
    /* djb2 hash function */
    size_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

bool hash_insert(SameHash *sh, size_t h, char *newkey) {
    SameHash *it = &sh[h];

    if (it->key == NULL) {
        it->key = newkey;
        return false;
    }

    do {
        if (!strcmp(it->key, newkey))
            return true;

        if (it->next)
            it = it->next;
        else
            break;
    } while (true);

    it->next = util_calloc(1, sizeof (SameHash));
    it->next->key = newkey;

    return false;
}
