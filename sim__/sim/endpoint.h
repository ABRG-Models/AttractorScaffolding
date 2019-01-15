#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_

/*!
 * For fitness debugging.
 */
#ifdef DEBUGF
# define DBGF(s)  cout << "DBGF: " << __FUNCTION__ << ": " << s << endl;
#else
# define DBGF(s)
#endif

/*!
 * An enumerated type for the end point of a cycle. Used in fitness functions.
 */
enum endpoint_t {
    ENDPOINT_UNKNOWN,
    ENDPOINT_LIMIT,
    ENDPOINT_POINT,
    ENDPOINT_N
};

#endif // _ENDPOINT_H_
