//! @file
//! @brief The rms16 source file.


/*---------------------------------------------------------------------*
 *  private: include files
 *---------------------------------------------------------------------*/

#include <math.h>

#include "rms16.h"


/*---------------------------------------------------------------------*
 *  private: definitions
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: typedefs
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: variables
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public:  variables
 *---------------------------------------------------------------------*/

const struct rms16_sc rms16 =
{
    rms16_add_signed,
    rms16_add_unsigned,
    rms16_clear,
    rms16_init,
};


/*---------------------------------------------------------------------*
 *  private: function prototypes
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: functions
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public:  functions
 *---------------------------------------------------------------------*/

bool rms16_add_signed(rms16_t * object, int16_t value)
{
    uint32_t index = object->index;
    int64_t sum = object->sum;
    uint64_t sum_square = object->sum_square;

    sum = sum + value;
    sum_square = sum_square + (uint32_t)((int32_t)value * value);

    index = index + 1;

    bool overflow = ( object->length == index );

    if(overflow)
    {
        double length_reciprocal = object->length_reciprocal;
        double mean = (double)sum * length_reciprocal;
        double mean_square = (double)sum_square * length_reciprocal;
        double alternating_square = mean_square - mean * mean;

        object->rms = sqrt(mean_square);
        object->mean = mean;

        if( 0 > alternating_square)
        {
            object->rms_alternating = -sqrt(-alternating_square);
        }
        else
        {
            object->rms_alternating = sqrt(alternating_square);
        }

        index = 0;
        sum = 0;
        sum_square = 0;

        if(object->on_calculated) { object->on_calculated(object); }
    }

    object->index = index;
    object->sum = sum;
    object->sum_square = sum_square;

    return overflow;
}

bool rms16_add_unsigned(rms16_t * object, uint16_t value)
{
    uint32_t index = object->index;
    int64_t sum = object->sum;
    uint64_t sum_square = object->sum_square;

    sum = sum + value;
    sum_square = sum_square + ((uint32_t)value * value);

    index = index + 1;

    bool overflow = ( object->length == index );

    if(overflow)
    {
        double length_reciprocal = object->length_reciprocal;
        double mean = (double)sum * length_reciprocal;
        double mean_square = (double)sum_square * length_reciprocal;
        double alternating_square = mean_square - mean * mean;

        object->rms = sqrt(mean_square);
        object->mean = mean;

        if( 0 > alternating_square)
        {
            object->rms_alternating = -sqrt(-alternating_square);
        }
        else
        {
            object->rms_alternating = sqrt(alternating_square);
        }

        index = 0;
        sum = 0;
        sum_square = 0;

        if(object->on_calculated) { object->on_calculated(object); }
    }

    object->index = index;
    object->sum = sum;
    object->sum_square = sum_square;

    return overflow;
}

void rms16_clear(rms16_t * object)
{
    object->rms = 0.0;
    object->mean = 0.0;
    object->rms_alternating = 0.0;
    object->sum = 0;
    object->sum_square = 0;
    object->index = 0;
}

void rms16_init(rms16_t * object, uint32_t length, double length_reciprocal)
{
    object->rms = 0.0;
    object->mean = 0.0;
    object->rms_alternating = 0.0;
    object->sum = 0;
    object->sum_square = 0;
    object->length = length;
    object->length_reciprocal = length_reciprocal;
    object->index = 0;
    object->on_calculated = NULL;
}


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
