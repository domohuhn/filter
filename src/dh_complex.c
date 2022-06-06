#include "dh/dh_complex.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <complex.h>


COMPLEX complex_unit_circle(double phi) {
    return MAKE_COMPLEX_NUMER(cos(phi),sin(phi));
}

