
#include "complex_bridge.h"

void split_complex_number_arrays(COMPLEX* in, double* real, double* imag, size_t size) {
    for(size_t i=0;i<size;++i) {
        real[i] = creal(in[i]);
        imag[i] = cimag(in[i]);
    }
}

