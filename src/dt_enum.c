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
static const int MIN_DT_COLOR_COUNT = 0;
static const int MAX_DT_COLOR_COUNT = sizeof(COLOR_NAMES) / sizeof(COLOR_NAMES[0]);

/*
 * dt_enum_is_valid returns true for a declared ordinal. C permits any integer
 * in an enumeration object. This function validates the declared range.
 */
bool dt_enum_is_valid(int ordinal)
{
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
    
    for ( int i = 0; i < MAX_DT_COLOR_COUNT; i++ ) {
        if ( (strcmp(name, COLOR_NAMES[i] ) == 0) ) {
            
            // assign matched color index to *out
            *out = i;
            return DT_OK;
        }
    }
    // if there's no matches 
    return DT_ERR_RANGE;
}