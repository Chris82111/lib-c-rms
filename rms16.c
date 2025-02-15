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
/*---------------------------------------------------------------------*
 *  private: function prototypes
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  private: functions
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public:  functions
 *---------------------------------------------------------------------*/

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
    object->calculated = 0;
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

bool rms16i_add(rms16_t * object, int16_t value)
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
        double mean = sum * length_reciprocal;
        double mean_square = sum_square * length_reciprocal;
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

        rms16_handler_t calculated =  object->calculated;
        if(NULL != calculated) { calculated(object); }
    }

    object->index = index;
    object->sum = sum;
    object->sum_square = sum_square;

    return overflow;
}

bool rms16u_add(rms16_t * object, uint16_t value)
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
        double mean = sum * length_reciprocal;
        double mean_square = sum_square * length_reciprocal;
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

        rms16_handler_t calculated =  object->calculated;
        if(NULL != calculated) { calculated(object); }
    }

    object->index = index;
    object->sum = sum;
    object->sum_square = sum_square;

    return overflow;
}


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
