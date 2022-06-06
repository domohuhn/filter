#ifndef DH_COMPLEX_H_INCLUDED
#define DH_COMPLEX_H_INCLUDED

/** @file
 * @brief Contains code to handle complex numbers in C, as MSVC does not support C99 complex types.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "complex.h"

#ifndef _WIN32
/** The complex data type for the computations. */
#define COMPLEX double _Complex
#define MAKE_COMPLEX_NUMER(re,im) ((re) + (im) * I)
#define COMPLEX_ADD(x,y) ((x)+(y))
#define COMPLEX_SUB(x,y) ((x)-(y))
#define COMPLEX_MUL(x,y) ((x)*(y))
#define COMPLEX_DIV(x,y) ((x)/(y))
#define COMPLEX_INV(x) (1.0/(x))
#define COMPLEX_NEG(x) (-(x))
#define COMPLEX_CONJ(x) MAKE_COMPLEX_NUMER(creal((x)), -cimag((x)))

#else

/** The complex data type for the computations. */
#define COMPLEX _Dcomplex
#define MAKE_COMPLEX_NUMER(re,im) _DCOMPLEX_((re),(im))

#define COMPLEX_ADD(x,y) MAKE_COMPLEX_NUMER((creal((x))+creal((y))), (cimag((x))+cimag((y))))
#define COMPLEX_SUB(x,y) MAKE_COMPLEX_NUMER((creal((x))-creal((y))), (cimag((x))-cimag((y))))
#define COMPLEX_MUL(x,y) _Cmulcc((x),(y))
#define COMPLEX_DIV(x,y) dh_complex_division((x),(y))
#define COMPLEX_INV(x) dh_complex_division(MAKE_COMPLEX_NUMER(1.0,0.0),(x))
#define COMPLEX_NEG(x) MAKE_COMPLEX_NUMER(-creal((x)), -cimag((x)))
#define COMPLEX_CONJ(x) MAKE_COMPLEX_NUMER(creal((x)), -cimag((x)))

inline COMPLEX dh_complex_division(COMPLEX num, COMPLEX deno) {
    double a =  creal(num);
    double b =  cimag(num);
    double x =  creal(deno);
    double y =  cimag(deno);
    double deno_sq = x*x+y*y;
    return MAKE_COMPLEX_NUMER( ((a*x+b*y)/deno_sq) , ((b*x-a*y)/deno_sq));
}

#endif

/** Returns a complex number on the complex unit circle located at the angle [phi]. */
COMPLEX complex_unit_circle(double phi);



#ifdef __cplusplus
}
#endif










#endif /* DH_COMPLEX_H_INCLUDED */


