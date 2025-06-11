//! @file
//! @brief The rms16 header file.
//!
//! @details The module can be used in C and C++.

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

//! @brief This macro initializes rms16 struct
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

//! @brief Forward typedef, for information see ::rms16_s
typedef struct rms16_s rms16_t;

//! @brief Function pointer as a handler that is called, see ::rms16_s.on_calculated
//!
//! @param[in,out] object The RMS object
typedef void (*rms16_handler_t)(rms16_t * object);

//! @brief The object data of the RMS object
typedef struct rms16_s
{
    //! @brief RMS value is updated before ::rms16_s::on_calculated is called.
    //! @details `rms = sqrt(rms_alternating * rms_alternating + mean * mean);`
    double rms;

    //! @brief Mean or DC component;
    double mean;

    //! @brief RMS of the alternating component
    double rms_alternating;

    //! @brief Sum value
    int64_t sum;

    //! @brief Sum of the square value
    uint64_t sum_square;

    //! @brief Number of measured values
    uint32_t length;

    //! @brief Reciprocal value of the length
    double length_reciprocal;

    //! @brief Index of the input values
    uint32_t index;

    //! @brief Function that is called after the last value is added
    rms16_handler_t on_calculated;

}rms16_t;

//! @brief Represents a simplified form of a class
//!
//! @details The global variable ::rms16 can be used to easily
//! access all matching functions with auto-completion.
struct rms16_sc
{
    bool (*AddSigned) (rms16_t * object, int16_t value); ///< @see ::rms16_add_signed()
    bool (*AddUnsigned) (rms16_t * object, uint16_t value); ///< @see ::rms16_add_unsigned()
    void (*Clear) (rms16_t * object); ///< @see ::rms16_clear()
    void (*Init) (rms16_t * object, uint32_t length, double length_reciprocal); ///< @see ::rms16_init()
};

/*---------------------------------------------------------------------*
 *  public: extern variables
 *---------------------------------------------------------------------*/

//! @brief To access all member functions working with type ::rms16_s
//!
//! @details Allows a simplified class to easily access all member functions
//! working with type ::rms16_s. The auto-completion function helps you
//! to select all suitable functions via ::rms16_sc struct.
extern const struct rms16_sc rms16;


/*---------------------------------------------------------------------*
 *  public: function prototypes
 *---------------------------------------------------------------------*/

//! @brief Adds a new measured value, signed
//!
//! @param[in,out] object The RMS object
//! @param value A new measured value
//! @retval true  if all elements have been read
//! @retval false if elements must be read
bool rms16_add_signed(rms16_t * object, int16_t value);

//! @brief Adds a new measured value, unsigned
//!
//! @param[in,out] object The RMS object
//! @param value A new measured value
//! @retval true  if all elements have been read
//! @retval false if elements must be read
bool rms16_add_unsigned(rms16_t * object, uint16_t value);

//! @brief Resets the RMS struct
//!
//! @param[in,out] object The RMS object
void rms16_clear(rms16_t * object);

//! @brief This function initializes rms16 struct
//!
//! @param[in,out] object The RMS object
//!
//! @param length Number of elements after the RMS value is calculated
//! @param length_reciprocal Reciprocal of the number of elements
void rms16_init(rms16_t * object, uint32_t length, double length_reciprocal);


/*---------------------------------------------------------------------*
 *  public: static inline functions
 *---------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* INC_RMS16_H_ */


/*---------------------------------------------------------------------*
 *  eof
 *---------------------------------------------------------------------*/
