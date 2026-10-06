/*
 * dt_map.c: Associative arrays for Unit 5, Section E.
 *
 * An array does not store its indices. This map stores its keys.
 * An array calculates a position with one subtraction.
 * The map calculates a hash and then compares keys in one bucket.
 *
 * Hashing turns the key into a bucket number. Compare all keys in that bucket
 * because two keys can select it. Use a linked list for each bucket. Start the
 * unsigned accumulator at 14695981039346656037ULL. For each unsigned byte,
 * exclusive-or the byte into it and multiply by 1099511628211ULL.
 *
 * A separate list stores insertion order for stable output. dt_map_key_at
 * reads this list. Updating a key preserves its position. Removing and
 * reinserting a key moves it to the end.
 */

#include "dt.h"

#include <stdlib.h>
#include <string.h>

#define MAP_BUCKETS 16

// Using SLL for bucket chains
struct dt_map_entry {
    char                 *key;
    dt_value             value;
    struct dt_map_entry  *next;     // next only
};

struct dt_map {
    struct dt_map_entry **entries;
    struct dt_map_entry **order;
    size_t entry_count;
    size_t order_capacity;
    size_t len;
};


/*
 * dt_map_new builds an empty map. It returns NULL after an allocation failure.
 */
dt_map *dt_map_new(void)
{
    dt_map *m = malloc(sizeof *m);
    if (!m) return NULL;

    // all buckets empty
    m-> entries = calloc(MAP_BUCKETS, sizeof *m->entries);
    if (m->entries == NULL) {
        free(m);
        return NULL;
    }

    m-> order = NULL;
    m-> entry_count = MAP_BUCKETS;
    m-> order_capacity = 0;
    m-> len = 0;
    return m;
}

/*
 * dt_map_free releases each entry, copied key, order array, and map.
 * It accepts NULL. The environment owns the values.
 */
void dt_map_free(dt_map *m)
{
    if (!m) return;

    for (size_t i = 0; i < (m->entry_count); i++) {
        struct dt_map_entry *e = m->entries[i];
        while (e != NULL) {
            struct dt_map_entry *next = e->next;
            free(e->key);
            free(e);
            e = next;
        }
        
    }
    free(m->order);
    free(m->entries);
    free(m);
}

/*
 * dt_map_len returns the number of keys in constant time.
 */
size_t dt_map_len(const dt_map *m)
{
    return m->len;
}

// hash 64-bit FNV-1a
static unsigned long long hash_func(const char *key) {
    unsigned long long h = 14695981039346656037ULL;
    for (const unsigned char *p = (const unsigned char *)key; *p != '\0'; p++) {
        h ^= (unsigned long long)*p;
        h *= 1099511628211ULL;
    }
    return h;
}


static size_t entry_index(const dt_map *m, const char *key)
{
    return ((size_t)(hash_func(key))) % (m -> entry_count);
}


/*
 * dt_map_put binds v to key.
 * An existing key keeps its insertion position. A new key becomes the last key.
 * Copy each new key because the caller owns the source buffer.
 * Return DT_ERR_CAPACITY after an allocation failure.
 */
dt_status dt_map_put(dt_map *m, const char *key, dt_value v)
{
    size_t bucket = (size_t)(hash_func(key) % m->entry_count);

    // for existing key, replace
    for (struct dt_map_entry *e = m->entries[bucket]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) {
            e->value = v;
            return DT_OK;
        }
    }

    // for new key, insert and expand array capacity
    if(m->len == m-> order_capacity) {
        size_t new_capacity;
        if (m->order_capacity == 0) {
            new_capacity =8;
        } else {
            new_capacity = m->order_capacity * 2;       // double the order capacity
        }

        struct dt_map_entry **grown = realloc(m->order, new_capacity * sizeof *grown);
        if (grown == NULL) {
            return DT_ERR_CAPACITY;
        }
        m->order = grown;
        m->order_capacity = new_capacity;
    }

    // allocate entry 
    struct dt_map_entry *e = malloc(sizeof *e);
    if (e == NULL) {
        return DT_ERR_CAPACITY;
    }

    size_t keylength = strlen(key);
    e->key = malloc(keylength + 1);

    // undo first allocation
    if (e->key == NULL) {
        free(e);
        return DT_ERR_CAPACITY;
    }

    memcpy(e->key, key, keylength + 1);
    e->value = v;

    // updated pointers for linking
    e->next = m->entries[bucket];
    m->entries[bucket] = e;
    m->order[m->len] = e;
    m->len++;
    return DT_OK;
}

/*
 * dt_map_get writes the value for key to *out.
 * It returns DT_ERR_KEY and does not change *out when the key is absent.
 * An absent key differs from a nil value.
 */
dt_status dt_map_get(const dt_map *m, const char *key, dt_value *out)
{
    // hash bucket number
    size_t bucket = (size_t)(hash_func(key) % m->entry_count);

    // compare keys in the same bucket
    for (struct dt_map_entry *e = m->entries[bucket]; e != NULL; e = e->next) {
        if (strcmp(e->key, key) == 0) {     // 0 means the text matches
            *out = e->value;
            return DT_OK;
        }
    }
    return DT_ERR_KEY;
}

/*
 * dt_map_remove removes key from its bucket and insertion position.
 * It releases the copied key. It returns DT_ERR_KEY when the key is absent.
 */
dt_status dt_map_remove(dt_map *m, const char *key)
{
    size_t e = entry_index(m, key);
    struct dt_map_entry **link = &m -> entries[e];
    while (*link) {
        if (strcmp((*link) -> key, key) == 0) {
            struct dt_map_entry *target = *link;
            *link = target -> next;


            size_t idx = 0; // idx since index is already being used
            while (m -> order[idx] != target) idx++;
            for (size_t i = idx; i + 1 < (m -> len); i++) {
                m -> order[i] = m -> order[i + 1]; // shift keys left
            }
            (m -> len)--;

            free(target -> key);
            free(target);
            return DT_OK;
        }
        link = &(*link) -> next;
    }

    return DT_ERR_KEY;
}

/*
 * dt_map_key_at writes the key at insertion position index to *out.
 * It returns DT_ERR_RANGE and does not change *out for an invalid index.
 */
dt_status dt_map_key_at(const dt_map *m, size_t index, const char **out)
{
    if (index >= m -> len) {
        return DT_ERR_RANGE;
    }
    *out = m-> order[index] -> key;
    return DT_OK;
}
