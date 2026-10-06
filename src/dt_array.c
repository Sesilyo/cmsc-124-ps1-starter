/*
 * dt_array.c: Array descriptors for Unit 5, Section D.
 *
 * Built-in C arrays use offsets that start at zero. Ada, Fortran, and Pascal
 * can use bounds such as 1..10 or -5..5. The index and offset then differ.
 *
 *     offset = index - lower_bound
 *
 * The run-time descriptor stores the lower bound for this subtraction.
 * The mathematical difference can exceed long long.
 * Confirm that the result is representable before you subtract signed values.
 *
 * Check both bounds. An index below the lower bound can access memory before
 * the allocation. That access has undefined behavior.
 */

#include "dt.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct dt_array {
    dt_value *elements;
    size_t    length;
    long long lower_bound;
};

/*
 * dt_array_new builds an array of length nil elements.
 * The first index is lower_bound. A zero length creates a valid empty array.
 * It returns NULL for an invalid size, invalid index range, or allocation failure.
 */
dt_array *dt_array_new(size_t length, long long lower_bound)
{
    // if new array
    if ( length > 0 ) {
        // reject if final index or allocation size can't be represented
        if ( ( lower_bound > 0 ) && ( (unsigned long long)(length - 1) > (unsigned long long)(LLONG_MAX - lower_bound) ))
            return NULL;
        if ( length > ( SIZE_MAX / sizeof(dt_value) ) ) return NULL;
    }

    dt_array *a = malloc(sizeof(dt_array));
    if (!a) return NULL;

    a -> elements = length ? calloc(length, sizeof(dt_value)) : NULL;
    if ( length && ( !a -> elements ) ) {
        free(a);
        return NULL;
    }

    for (size_t i = 0; i < length; i++) {
        a -> elements[i] = dt_value_nil();
    }

    a -> length = length;
    a -> lower_bound = lower_bound;
    return a;
}

/*
 * dt_array_free releases the element block and descriptor. It accepts NULL.
 * The environment owns the runtime objects referenced by the dt_value elements.
 */
void dt_array_free(dt_array *a)
{
    // if array a does not exist
    if (!a) return;

    // release elements
    free(a -> elements);
    
    // release descriptor
    free(a);
}

/*
 * dt_array_len returns the stored element count in constant time.
 */

 // AKA get_length()
size_t dt_array_len(const dt_array *a)
{
    return (a -> length);
}

/*
 * dt_array_lower_bound returns the first array index. With lower bound 1,
 * index 1 uses storage offset 0.
 */

// AKA get_lower_bound()
long long dt_array_lower_bound(const dt_array *a)
{
    return (a -> lower_bound);
}


static bool array_offset(const dt_array *a, long long index, size_t *offset_out) {
    if ( index < (a -> lower_bound) ) return false;

    unsigned long long distance = (unsigned long long)index - (unsigned long long)(a -> lower_bound);
    if ( distance >= (unsigned long long)( a -> length ) ) return false;

    *offset_out = (size_t)distance;
    return true;
}


/*
 * dt_array_get writes the element at index to *out.
 * It returns DT_ERR_RANGE and does not change *out for an invalid index.
 */
dt_status dt_array_get(const dt_array *a, long long index, dt_value *out)
{
    size_t offset;
    if ( !array_offset(a, index, &offset) ) return DT_ERR_RANGE;

    *out = a -> elements[offset];
    return DT_OK;
}

/*
 * dt_array_set replaces the element at index with v.
 * It returns DT_ERR_RANGE and changes nothing for an invalid index.
 * The environment keeps ownership of the old value.
 */
dt_status dt_array_set(dt_array *a, long long index, dt_value v)
{
    size_t offset;
    if ( !array_offset(a, index, &offset) ) return DT_ERR_RANGE;
    
    a -> elements[offset] = v;
    return DT_OK;
}
