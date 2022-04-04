# Chebyshev filters

Here are some exmaples of chebyshev filters.

# Lowpass

| Name             | Value          |
|------------------|----------------|
| Order            | 4              |
| Filter type        | 'lowpass'    |
| Sampling frequency | 100 Hz       |
| Cutoff 1           | 25 Hz        |
| Ripple           | -3 dB        |

![Lowpass](chebyshev_lowpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create chebyshev filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_CHEBYSHEV_LOWPASS;
opts.parameters.chebyshev.cutoff_frequency_hz = 25.0;
opts.parameters.chebyshev.sampling_frequency_hz = 100.0;
opts.parameters.chebyshev.filter_order = 4;
opts.parameters.chebyshev.ripple = -3.0;
if ( dh_create_filter(&filter_data,&opts) != DH_FILTER_OK) {
    // handle error
}
```

# Highpass

| Name             | Value          |
|------------------|----------------|
| Order            | 4              |
| Filter type        | 'highpass'    |
| Sampling frequency | 100 Hz       |
| Cutoff 1           | 25 Hz        |
| Ripple           | -3 dB        |

![Highpass](chebyshev_highpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create chebyshev filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_CHEBYSHEV_HIGHPASS;
opts.parameters.chebyshev.cutoff_frequency_hz = 25.0;
opts.parameters.chebyshev.sampling_frequency_hz = 100.0;
opts.parameters.chebyshev.filter_order = 4;
opts.parameters.chebyshev.ripple = -3.0;
if ( dh_create_filter(&filter_data,&opts) != DH_FILTER_OK) {
    // handle error
}
```

# Bandpass

| Name             | Value          |
|------------------|----------------|
| Order            | 4              |
| Filter type        | 'bandpass'    |
| Sampling frequency | 100 Hz       |
| Cutoff 1           | 15 Hz        |
| Cutoff 2           | 35 Hz        |
| Ripple           | -3 dB        |

![Bandpass](chebyshev_bandpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create chebyshev filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_CHEBYSHEV_BANDPASS;
opts.parameters.chebyshev.cutoff_frequency_hz = 15.0;
opts.parameters.chebyshev.cutoff_frequency_2_hz = 35.0;
opts.parameters.chebyshev.sampling_frequency_hz = 100.0;
opts.parameters.chebyshev.filter_order = 4;
opts.parameters.chebyshev.ripple = -3.0;
if ( dh_create_filter(&filter_data,&opts) != DH_FILTER_OK) {
    // handle error
}
```

# Bandstop

| Name             | Value          |
|------------------|----------------|
| Order            | 4              |
| Filter type        | 'bandstop'    |
| Sampling frequency | 100 Hz       |
| Cutoff 1           | 15 Hz        |
| Cutoff 2           | 35 Hz        |
| Ripple           | -3 dB        |

![Bandstop](chebyshev_bandstop.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create chebyshev filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_CHEBYSHEV_BANDSTOP;
opts.parameters.chebyshev.cutoff_frequency_hz = 15.0;
opts.parameters.chebyshev.cutoff_frequency_2_hz = 35.0;
opts.parameters.chebyshev.sampling_frequency_hz = 100.0;
opts.parameters.chebyshev.filter_order = 4;
opts.parameters.chebyshev.ripple = -3.0;
if ( dh_create_filter(&filter_data,&opts) != DH_FILTER_OK) {
    // handle error
}
```
