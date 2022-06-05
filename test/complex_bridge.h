#ifndef C_CPP_COMPLEX_BRIDGE_INCLUDED
#define C_CPP_COMPLEX_BRIDGE_INCLUDED

#include "dh/utility.h"

#ifdef __cplusplus
extern "C" {
#endif

void split_complex_number_arrays(COMPLEX* in, double* real, double* imag, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* GENERATED_C_CODE_INCLUDED */
