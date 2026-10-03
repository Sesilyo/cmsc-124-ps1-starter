/*
 * dt_enum.c: Enumerations for Unit 5, Section C.
 *
 * A C enumeration type is compatible with an integer type and uses named
 * enumerators. A dt_color can still hold 47. This module instead accepts only
 * the three declared color ordinals. Other languages place different
 * restrictions on creating enumeration values from arbitrary integers.
 *
 * These three functions validate each enumeration operation in one location.
 */

#include "dt.h"

#include <string.h>

static const char *const COLOR_NAMES[] = { "RED", "GREEN", "BLUE" };
const MIN_DT_COLOR_COUNT = 0;
const MAX_DT_COLOR_COUNT = sizeof(COLOR_NAMES) / sizeof(COLOR_NAMES[0]);

/*
 * dt_enum_is_valid returns true for a declared ordinal. C permits any integer
 * in an enumeration object. This function validates the declared range.
 */
bool dt_enum_is_valid(int ordinal)
    /* parameters: () 
     * check every other function through
     *
     */
{
    /* TODO: Return true for an ordinal from zero through DT_COLOR_COUNT - 1.
    dt_enum_is_valid(0)   -> true, RED
    dt_enum_is_valid(2)   -> true, BLUE
    dt_enum_is_valid(3)   -> false, one past the set
    dt_enum_is_valid(-1)  -> false, below the lower bound */

    /*  first bool expression:  checks if ordinal is equal or greater than min size
     *  second bool expression: checks if ordinal is less than max size
     *  returns true/false
     */
    return ((ordinal >= MIN_DT_COLOR_COUNT) && (ordinal < MAX_DT_COLOR_COUNT));
}

/*
 * dt_enum_name writes the enumerator text to *out. It returns DT_ERR_RANGE for
 * an invalid ordinal. A failure preserves *out.
 */
dt_status dt_enum_name(int ordinal, const char **out)
{
    /* TODO: Return DT_ERR_RANGE when the ordinal is outside the set.
        Otherwise, write the corresponding text to *out.
        Validate the ordinal before you index COLOR_NAMES.
        dt_enum_name(0, &out)  -> DT_OK, *out = "RED"
        dt_enum_name(2, &out)  -> DT_OK, *out = "BLUE"
        dt_enum_name(3, &out)  -> DT_ERR_RANGE, *out untouched
cases/normal/enum_names.case */
    
    /*  [1] checks if ordinal is valid
     *  [2] if not return DT_ERR_RANGE
     *  [3] if valid, assign to COLOR_NAME[ordinal] to *out
     *  [4] return DT_OK
     */  
    if (!dt_enum_is_valid(ordinal)) return DT_ERR_RANGE;
    *out = COLOR_NAMES[ordinal];
    return DT_OK;
}

/*
 * dt_enum_from_name searches the enumerator text and writes its ordinal to
 * *out. It returns DT_ERR_RANGE when the text has no match.
 */
dt_status dt_enum_from_name(const char *name, int *out)
{
    /* TODO: Search COLOR_NAMES. Return DT_ERR_RANGE when no text matches.
        dt_enum_from_name("GREEN", &out)   -> DT_OK, out = 1
        dt_enum_from_name("PURPLE", &out)  -> DT_ERR_RANGE, out untouched
        dt_enum_from_name("1", &out)       -> DT_ERR_RANGE because no text matches
cases/normal/enum_names.case */
    
    for ( int i = 0; i < MAX_DT_COLOR_COUNT; i++ ) {
        if ( (strcmp(name, COLOR_NAMES[i] ) == 0) ) {
            /* strcmp(str_1, str_2) compares 2 strings 
             * returns 0 if match
             * if not match:
             *      returns negative if str_1[0] < str_2[0]
             *      returns positive if str_1[0] > str_2[0] 
             */

            // assign matched color index to *out
            *out = i;
            return DT_OK;
        }
    }
    // if there's no matches 
    return DT_ERR_RANGE;
}