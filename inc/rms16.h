//! @file

#ifndef INC_RMS16_H_
#define INC_RMS16_H_

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------*
 *  public: include files
 *---------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/*---------------------------------------------------------------------*
 *  public: define
 *---------------------------------------------------------------------*/

//! @brief This macro initializes rms16 struct.
//!
#define RMS16_INIT(LENGTH, LENGTH_RECIPROCAL)     \
{                                                 \
    /* .rms               */ (0.0),               \
    /* .mean              */ (0.0),               \
    /* .rms_alternating   */ (0.0),               \
    /* .sum               */ (0),                 \
    /* .sum_square        */ (0),                 \
    /* .length            */ (LENGTH),            \
    /* .length_reciprocal */ (LENGTH_RECIPROCAL), \
    /* .index             */ (0),                 \
    /* .on_calculated     */ (NULL),              \
}


/*---------------------------------------------------------------------*
 *  public: typedefs
 *---------------------------------------------------------------------*/

//! @brief Forward declaration
struct rms16_s;

//! @brief Prototype of `rms16_t`, for information see `rms16_s`
//!
typedef struct rms16_s rms16_t;

//! @brief Function pointer as a handler that is called, see `rms16_t.calculated`
//! @param[in,out] object The RMS object
//!
typedef void (*rms16_handler_t)(rms16_t * object);

//! @brief The object data of the RMS object
//!
typedef struct rms16_s
{

    //! RMS value is updated before `calculated` is called.
    //! `rms = sqrt(rms_alternating * rms_alternating + mean * mean);`
    double rms;

    //! Mean or DC component;
    double mean;

    //! RMS of the alternating component
    double rms_alternating;

    //! Sum value
    int64_t sum;

    //! Sum of the square value
    uint64_t sum_square;

    //! Number of measured values
    uint32_t length;

    //! Reciprocal value of the length
    double length_reciprocal;

    //! Index of the input values
    uint32_t index;

    //! Function that is called after the last value is added
    rms16_handler_t on_calculated;

}rms16_t;


/*---------------------------------------------------------------------*
 *  public: extern variables
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  public: function prototypes
 *---------------------------------------------------------------------*/

void rms16_init(rms16_t * object, uint32_t length, double length_reciprocal);
void rms16_clear(rms16_t * object);
bool rms16u_add(rms16_t * object, uint16_t value);
bool rms16i_add(rms16_t * object, int16_t value);


/*---------------------------------------------------------------------*
 *  public: static inline functions
 *---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* INC_RMS16_H_ */
