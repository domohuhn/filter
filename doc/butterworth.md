# Butterworth filters

Here are some exmaples of butterworth filters.

# Lowpass

| Name             | Value          |
|------------------|----------------|
| Order            | 4              |
| Filter type        | 'lowpass'    |
| Sampling frequency | 100 Hz       |
| Cutoff 1           | 25 Hz        |

![Lowpass](butterworth_lowpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create butterworth filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_BUTTERWORTH_LOWPASS;
opts.parameters.butterworth.cutoff_frequency_hz = 25.0;
opts.parameters.butterworth.sampling_frequency_hz = 100.0;
opts.parameters.butterworth.filter_order = 4;
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

![Highpass](butterworth_highpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create butterworth filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_BUTTERWORTH_HIGHPASS;
opts.parameters.butterworth.cutoff_frequency_hz = 25.0;
opts.parameters.butterworth.sampling_frequency_hz = 100.0;
opts.parameters.butterworth.filter_order = 4;
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

![Bandpass](butterworth_bandpass.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create butterworth filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_BUTTERWORTH_BANDPASS;
opts.parameters.butterworth.cutoff_frequency_hz = 15.0;
opts.parameters.butterworth.cutoff_frequency_2_hz = 35.0;
opts.parameters.butterworth.sampling_frequency_hz = 100.0;
opts.parameters.butterworth.filter_order = 4;
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

![Bandstop](butterworth_bandstop.png)

How to create this filter in code:
```c
#include "dh/filter.h"

// create butterworth filter
dh_filter_data filter_data;
dh_filter_options opts;
opts.filter_type = DH_IIR_BUTTERWORTH_BANDSTOP;
opts.parameters.butterworth.cutoff_frequency_hz = 15.0;
opts.parameters.butterworth.cutoff_frequency_2_hz = 35.0;
opts.parameters.butterworth.sampling_frequency_hz = 100.0;
opts.parameters.butterworth.filter_order = 4;
if ( dh_create_filter(&filter_data,&opts) != DH_FILTER_OK) {
    // handle error
}
```
