
#include "dh/butterworth.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "stdlib.h"
#include "complex.h"
#include "dh/utility.h"


static size_t butterworth_splane_zeros(COMPLEX* a,size_t b,size_t c,void* d) {
    return 0;
}

static size_t butterworth_splane_poles(COMPLEX* splane,size_t count,size_t order,void* user) {
    dh_compute_poles_on_s_plane(splane,order);
    return order;
}

DH_FILTER_RETURN_VALUE compute_butterworth_filter_coefficients(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC characteristic)
{
    if(filter == NULL || options == NULL || filter->coefficients_in == NULL || filter->coefficients_out == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    dh_transfer_function_callbacks cbs;
    cbs.characteristic = characteristic;
    cbs.zeros = &butterworth_splane_zeros;
    cbs.poles = &butterworth_splane_poles;
    cbs.user_data = NULL;
    return dh_compute_transfer_function_polynomials(filter,options,cbs);
}
