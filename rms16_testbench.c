/*---------------------------------------------------------------------*
 *  private: include files
 *---------------------------------------------------------------------*/

#include <inttypes.h>

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

static void run_example(void);
static uint8_t rms16_test_init_initialize(void);
static uint8_t rms16_test_init(void);
static void rms16_test_handler(rms16_t * object);
static uint8_t rms16_test_handler_test(void);
static uint8_t rms16_test_calculation(void);
static uint8_t rms16_test_max_length(void);

void rms16_test(void);


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
        if(rms16u_add(&rms_object, input_data[i]))
        {
            rms = rms_object.rms; // 1.5811388...
            rms_alternating = rms_object.rms_alternating; // 0.5
            mean = rms_object.mean; // 1.5
        }
    }

    // Solves `variable set but not used`
    rms = rms_alternating;
    rms_alternating = mean;
    mean = rms;
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
    if( 0 != rms.calculated )
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
    if( 0 != rms.calculated )
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

    rms.calculated = rms16_test_handler;

    if( rms16_test_handler != rms.calculated )
    {
        errors++;
    }

    rms16u_add(ptr, 1);
    rms16u_add(ptr, 2);
    rms16u_add(ptr, 3);
    rms16u_add(ptr, 4);

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
    rms16u_add(ptr, 1);
    rms16u_add(ptr, 2);
    rms16u_add(ptr, 3);
    rms16u_add(ptr, 4);
    if( 2.7386127875258306 != rms.rms )
    {
        errors++;
    }

    rms16_init(ptr, 5, 0.2);
    rms16u_add(ptr, 1);
    rms16u_add(ptr, 2);
    rms16u_add(ptr, 3);
    rms16u_add(ptr, 4);
    rms16u_add(ptr, 5);
    if( 3.3166247903553998 != rms.rms )
    {
        errors++;
    }

    rms16_t rms_offset;
    rms16_init(&rms_offset, 6, 1.0/6.0);
    rms16u_add(&rms_offset, 1);
    rms16u_add(&rms_offset, 2);
    rms16u_add(&rms_offset, 3);
    rms16u_add(&rms_offset, 3);
    rms16u_add(&rms_offset, 2);
    rms16u_add(&rms_offset, 1);
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
    rms16i_add(&rms_ac, -1);
    rms16i_add(&rms_ac, 0);
    rms16i_add(&rms_ac, 1);
    rms16i_add(&rms_ac, 1);
    rms16i_add(&rms_ac, 0);
    rms16i_add(&rms_ac, -1);
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
    rms.calculated = rms16_test_handler;

    rms16_init(ptr, 0, 1.0 / ( UINT64_C(1) << 32 ));

    for(uint32_t i = 0; i < UINT32_MAX/2; i++)
    {
        rms16u_add(ptr, UINT16_MAX);
    }

    for(uint32_t i = 0; i < UINT32_MAX/2; i++)
    {
        rms16u_add(ptr, UINT16_MAX);
    }

    bool overflow = rms16u_add(ptr, UINT16_MAX);

    if( true == overflow )
    {
        errors++;
    }

    overflow = rms16u_add(ptr, UINT16_MAX);

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


/*---------------------------------------------------------------------*
 *  public:  functions
 *---------------------------------------------------------------------*/

/*
#ifdef __cplusplus
extern "C" {
#endif

extern void rms16_test(void);

#ifdef __cplusplus
}
#endif
*/

void rms16_test(void)
{
    run_example();

    uint8_t errors = 0;

    errors += rms16_test_init_initialize();
    errors += rms16_test_init();
    errors += rms16_test_handler_test();
    errors += rms16_test_calculation();
    errors += rms16_test_max_length();
}


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
