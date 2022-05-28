#include "dh/chebyshev.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "complex.h"
#include "dh/utility.h"

static void dh_transform_s_poles_to_chebyshev(COMPLEX* ptr, size_t len, double ripple) {
    double ripple_power = pow(10.0, -ripple / 10.0);
	double tmp = sqrt(ripple_power - 1.0);
	double arg = asinh(1.0 / tmp) / (double) len;
    for (int i = 0; i < len; i++)
	{ 
        double real = creal(ptr[i]);
        double imag = cimag(ptr[i]);
        ptr[i] = real*sinh(arg) + imag * cosh(arg) * I;
	}
}


typedef struct 
{
    double ripple_db;
    bool isType2;
} dh_chebyshev_data;

static size_t chebyshev_splane_zeros(COMPLEX* splane,size_t count,size_t order,void* user) {
    size_t number_zeros = 0;
    dh_chebyshev_data* data = (dh_chebyshev_data*)user;
    if(data->isType2) {
        for(size_t i=0; i<order; ++i) {
            double phi = (2*i+1)*M_PI/(2*order);
            splane[i] = I/cos(phi);
        }
        number_zeros = order;
    }
    return number_zeros;
}

static size_t chebyshev_splane_poles(COMPLEX* splane,size_t count,size_t order,void* user) {
    dh_chebyshev_data* data = (dh_chebyshev_data*)user;
    dh_compute_poles_on_s_plane(splane,order);
    dh_transform_s_poles_to_chebyshev(splane,order,data->ripple_db);
    // convert to chebyshev type 2
    if(data->isType2) {
        for(size_t i=0; i< order; ++i){
            splane[i] = 1.0/splane[i];
        }
    }
    return order;
}

DH_FILTER_RETURN_VALUE compute_chebyshev_filter_coefficients(dh_filter_data* filter, dh_filter_parameters* options, DH_FILTER_CHARACTERISTIC characteristic, bool isType2)
{
    if(filter == NULL || options == NULL || filter->coefficients_in == NULL || filter->coefficients_out == NULL) {
        return DH_FILTER_NO_DATA_STRUCTURE;
    }
    dh_transfer_function_callbacks cbs;
    cbs.characteristic = characteristic;
    cbs.zeros = &chebyshev_splane_zeros;
    cbs.poles = &chebyshev_splane_poles;
    dh_chebyshev_data data;
    data.isType2 = isType2;
    data.ripple_db = options->ripple;
    cbs.user_data = &data;
    return dh_compute_transfer_function_polynomials(filter,options,cbs);
}
