typedef struct HashTable HashTable;

bool hash_insert(HashTable *, char *, size_t);
HashTable *hash_table_create(size_t);
void hash_table_destroy(HashTable *);
