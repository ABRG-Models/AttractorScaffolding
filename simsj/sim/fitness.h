/*
 * This is a header used to selectively select one of 2 fitness
 * functions.
 */

#ifndef __FITNESS_H__
#define __FITNESS_H__

// For fitness debugging.
#ifdef DEBUGF
# define DBGF(s)  cout << "DBGF: " << __FUNCTION__ << ": " << s << endl;
#else
# define DBGF(s)
#endif

/*
 * An enumerated type for the end point of a cycle. Used in fitness functions.
 */
enum endpoint_t {
    ENDPOINT_UNKNOWN,
    ENDPOINT_LIMIT,
    ENDPOINT_POINT,
    ENDPOINT_N
};

// The default is fitness1.h; pass -DUSE_FITNESS_0 in compiler command
// line to select fitness0.h.
#if defined USE_FITNESS_0
// The fitness function created by Stuart Wilson
# include "fitness0.h"

#elif defined USE_FITNESS_2
// Another fitness function, explored by Seb James
# include "fitness2.h"

#elif defined USE_FITNESS_3
// Another fitness function, explored by Seb
# include "fitness3.h"

#elif defined USE_FITNESS_4
// Stuart's breakthrough fitness function, considering limit cycle as
// defining "proportional expression".
# include "fitness4.h"

#else
// The fitness function created by Dan Whiteley
# include "fitness1.h"
#endif

#endif // __FITNESS_FUNCTION__
