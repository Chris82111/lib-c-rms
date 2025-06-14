/*---------------------------------------------------------------------*
 *  private: include files
 *---------------------------------------------------------------------*/

#include <inttypes.h>

#include "rms16.h"


/*---------------------------------------------------------------------*
 *  private: definitions
 *---------------------------------------------------------------------*/

#define ELEMENTS_OF(ARRAY) ( sizeof(ARRAY) / sizeof((ARRAY)[0]) )


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

static void run_example(void);
static uint8_t rms16_test_init_initialize(void);
static uint8_t rms16_test_init(void);
static void rms16_test_handler(rms16_t * object);
static uint8_t rms16_test_handler_test(void);
static uint8_t rms16_test_calculation(void);
static uint8_t rms16_test_max_length(void);


/*---------------------------------------------------------------------*
 *  private: functions
 *---------------------------------------------------------------------*/

static void run_example(void)
{
    double rms, rms_alternating, mean;

    rms16_t rms_object = RMS16_INIT(
        2,  // LENGTH
        0.5 // LENGTH_RECIPROCAL
    );

    uint16_t input_data[] = { 2, 1 };

    for(uint8_t i = 0; i < rms_object.length; i++)
    {
        if(rms16_add_unsigned(&rms_object, input_data[i]))
        {
            rms = rms_object.rms; // 1.5811388...
            rms_alternating = rms_object.rms_alternating; // 0.5
            mean = rms_object.mean; // 1.5
        }
    }

    // Suppresses the warning, variable is not used
    if(0 == rms || 0 == rms_alternating || 0 == mean){ ; }
}

static uint8_t rms16_test_init_initialize(void)
{
    uint8_t errors = 0;

    rms16_t rms = RMS16_INIT(8, 0.125);
    if( 0.0 != rms.rms )
    {
        errors++;
    }
    if( 0.0 != rms.mean )
    {
        errors++;
    }
    if( 0.0 != rms.rms_alternating )
    {
        errors++;
    }
    if( 0 != rms.sum )
    {
        errors++;
    }
    if( 0 != rms.sum_square )
    {
        errors++;
    }
    if( 8 != rms.length )
    {
        errors++;
    }
    if( 0.125 != rms.length_reciprocal )
    {
        errors++;
    }
    if( 0 != rms.index )
    {
        errors++;
    }
    if( NULL != rms.on_calculated )
    {
        errors++;
    }

    return errors;
}

static uint8_t rms16_test_init(void)
{
    uint8_t errors = 0;

    rms16_t rms;
    rms16_t * ptr = &rms;

    rms16_init(ptr, 4, 0.25);
    if( 0.0 != rms.rms )
    {
        errors++;
    }
    if( 0.0 != rms.mean )
    {
        errors++;
    }
    if( 0.0 != rms.rms_alternating )
    {
        errors++;
    }
    if( 0 != rms.sum )
    {
        errors++;
    }
    if( 0 != rms.sum_square )
    {
        errors++;
    }
    if( 4 != rms.length )
    {
        errors++;
    }
    if( 0.25 != rms.length_reciprocal )
    {
        errors++;
    }
    if( 0 != rms.index )
    {
        errors++;
    }
    if( NULL != rms.on_calculated )
    {
        errors++;
    }

    return errors;
}

static void rms16_test_handler(rms16_t * object)
{
    object->length++;
    // Does not work in general, 0 is permissible for the length. 0 corresponds to 2^32.
    object->length_reciprocal = 1.0 / object->length;
}

static uint8_t rms16_test_handler_test(void)
{
    uint8_t errors = 0;

    rms16_t rms;
    rms16_t * ptr = &rms;
    rms16_init(ptr, 4, 0.25);

    rms.on_calculated = rms16_test_handler;

    if( rms16_test_handler != rms.on_calculated )
    {
        errors++;
    }

    rms16_add_unsigned(ptr, 1);
    rms16_add_unsigned(ptr, 2);
    rms16_add_unsigned(ptr, 3);
    rms16_add_unsigned(ptr, 4);

    if( 5 != rms.length )
    {
        errors++;
    }

    return errors;
}

static uint8_t rms16_test_calculation(void)
{
    uint8_t errors = 0;

    rms16_t rms;
    rms16_t * ptr = &rms;

    rms16_init(ptr, 4, 0.25);
    rms16_add_unsigned(ptr, 1);
    rms16_add_unsigned(ptr, 2);
    rms16_add_unsigned(ptr, 3);
    rms16_add_unsigned(ptr, 4);
    if( 2.7386127875258306 != rms.rms )
    {
        errors++;
    }

    rms16_init(ptr, 5, 0.2);
    rms16_add_unsigned(ptr, 1);
    rms16_add_unsigned(ptr, 2);
    rms16_add_unsigned(ptr, 3);
    rms16_add_unsigned(ptr, 4);
    rms16_add_unsigned(ptr, 5);
    if( 3.3166247903553998 != rms.rms )
    {
        errors++;
    }

    rms16_t rms_offset;
    rms16_init(&rms_offset, 6, 1.0/6.0);
    rms16_add_unsigned(&rms_offset, 1);
    rms16_add_unsigned(&rms_offset, 2);
    rms16_add_unsigned(&rms_offset, 3);
    rms16_add_unsigned(&rms_offset, 3);
    rms16_add_unsigned(&rms_offset, 2);
    rms16_add_unsigned(&rms_offset, 1);
    if( 2.1602468994692865 != rms_offset.rms )
    {
        errors++;
    }
    if( 2 != rms_offset.mean )
    {
        errors++;
    }
    if( 0.8164965809277257 != rms_offset.rms_alternating )
    {
        errors++;
    }

    rms16_t rms_ac;
    rms16_init(&rms_ac, 6, 1.0/6.0);
    rms16_add_signed(&rms_ac, -1);
    rms16_add_signed(&rms_ac, 0);
    rms16_add_signed(&rms_ac, 1);
    rms16_add_signed(&rms_ac, 1);
    rms16_add_signed(&rms_ac, 0);
    rms16_add_signed(&rms_ac, -1);
    if( 0.81649658092772603 != rms_ac.rms )
    {
        errors++;
    }
    if( 0 != rms_ac.mean )
    {
        errors++;
    }
    if( 0.81649658092772603 != rms_ac.rms_alternating )
    {
        errors++;
    }

    return errors;
}

static uint8_t rms16_test_max_length(void)
{
    uint8_t errors = 0;

    rms16_t rms;
    rms16_t * ptr = &rms;
    rms.on_calculated = rms16_test_handler;

    rms16_init(ptr, 0, 1.0 / ( UINT64_C(1) << 32 ));

    for(uint32_t i = 0; i < UINT32_MAX/2; i++)
    {
        rms16_add_unsigned(ptr, UINT16_MAX);
    }

    for(uint32_t i = 0; i < UINT32_MAX/2; i++)
    {
        rms16_add_unsigned(ptr, UINT16_MAX);
    }

    bool overflow = rms16_add_unsigned(ptr, UINT16_MAX);

    if( true == overflow )
    {
        errors++;
    }

    overflow = rms16_add_unsigned(ptr, UINT16_MAX);

    if( false == overflow )
    {
        errors++;
    }

    if( 65535 != rms.rms )
    {
        errors++;
    }

    return errors;
}

static void rms16_test_sin_calculation(void)
{
    double rms, rms_alternating, mean;

    // 1.65 V DC + 1 V * sqrt(2) * sin( t / 20 ms * 2 * PI ) (@ 4096)
    uint16_t input[20] = { 2048, 2590, 3080, 3468, 3717, 3803, 3717, 3468, 3080, 2590, 2048, 1506, 1016, 628, 379, 293, 379, 628, 1016, 1506 };

    rms16_t rms_object = RMS16_INIT(
        ELEMENTS_OF(input),
        1.0 / ELEMENTS_OF(input)
    );

    for(uint16_t i = 0; i < rms_object.length; i++)
    {
        if(rms16.AddUnsigned(&rms_object, input[i]))
        {
            rms = rms_object.rms;
            rms_alternating = rms_object.rms_alternating;
            mean = rms_object.mean;
        }
    }

    // 1.65 V DC + 1 V * sqrt(2) * sin( t * 60 Hz * 2 * PI ) (@ 4096)
    uint16_t input_3_times_60_hz[] = { 2048, 2694, 3250, 3636, 3800, 3717, 3401, 2894, 2268, 1611, 1016, 566, 324, 324, 566, 1016, 1611, 2268, 2894, 3401, 3717, 3800, 3636, 3250, 2694, 2048, 1402, 846, 460, 296, 379, 695, 1202, 1828, 2485, 3080, 3530, 3772, 3772, 3530, 3080, 2485, 1828, 1202, 695, 379, 296, 460, 846, 1402 };

    rms16.Init(&rms_object,
        ELEMENTS_OF(input_3_times_60_hz),
        1.0 / ELEMENTS_OF(input_3_times_60_hz)
    );

    for(uint16_t i = 0; i < rms_object.length; i++)
    {
        if(rms16.AddUnsigned(&rms_object, input_3_times_60_hz[i]))
        {
            rms = rms_object.rms;
            rms_alternating = rms_object.rms_alternating;
            mean = rms_object.mean;
        }
    }

    // 1.65 V DC + 1 V * sqrt(2) * sin( t * 60 Hz * 2 * PI ) (@ 4096) with interference
    int16_t input_3_times_60_hz_interference[] = { 1706, 3240, 2681, 3524, 3462, 3112, 3572, 3132, 1833, 1781, 1469, 148, 868, -197, 345, 594, 1259, 2749, 2953, 3663, 3913, 3562, 3213, 2753, 3070, 1581, 1785, 1143, 126, -108, 329, 1308, 1596, 2364, 1968, 2654, 3543, 3808, 3603, 4020, 3054, 2166, 1870, 760, 1241, 168, -139, 310, 1260, 1470 };

    rms16.Init(&rms_object,
        ELEMENTS_OF(input_3_times_60_hz_interference),
        1.0 / ELEMENTS_OF(input_3_times_60_hz_interference)
    );

    for(uint16_t i = 0; i < rms_object.length; i++)
    {
        if(rms16.AddSigned(&rms_object, input_3_times_60_hz_interference[i]))
        {
            rms = rms_object.rms;
            rms_alternating = rms_object.rms_alternating;
            mean = rms_object.mean;
        }
    }

    if(0 == rms || 0 == rms_alternating || 0 == mean){ ; }
}


/*---------------------------------------------------------------------*
 *  public:  functions
 *---------------------------------------------------------------------*/

void rms16_test(void)
{
    run_example();

    uint8_t errors = 0;

    errors += rms16_test_init_initialize();
    errors += rms16_test_init();
    errors += rms16_test_handler_test();
    errors += rms16_test_calculation();
    errors += rms16_test_max_length();

    rms16_test_sin_calculation();
}


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
