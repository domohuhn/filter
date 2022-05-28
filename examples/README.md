# Examples

## Program "Design Filter"

This example is a command line program that can be used to design a filter and to generate the example graphs in the doc folder.
It shows how the C++ interface can be used. 

Usage:
```
design-filter [OPTION...]
  -p, --parameter-type arg      Filter type (one of no-filter, butterworth,
                                chebyshev, moving-average, brickwall)
                                (default: butterworth)
  -t, --type arg                Filter type (one of lowpass, highpass,
                                bandpass, bandstop) (default: lowpass)
  -o, --order arg               Order of the filter (default: 4)
  -g, --graphs                  Create data for graphs
  -s, --sampling-frequency arg  Sampling frequency in Hz (default: 100)
  -c, --cutoff-frequency arg    Cutoff frequencies in Hz (default: 10,20)
  -r, --ripple arg              Ripple in dB for chebyshev (default: 3.0)
  -h, --help                    Print usage
```

Example calls to the program showing different filter types:
```
.\design-filter.exe -p moving-average -o 15 -t lowpass -g 
.\design-filter.exe -p moving-average -o 15 -t highpass -g
.\design-filter.exe -p exponential -t lowpass -c 15 -g
.\design-filter.exe -p butterworth -o 4 -t bandpass -c 15,35 -g
.\design-filter.exe -p butterworth -o 4 -t bandstop -c 15,35 -g
.\design-filter.exe -p butterworth -o 4 -t lowpass -c 25 -g
.\design-filter.exe -p butterworth -o 4 -t highpass -c 25 -g
.\design-filter.exe -p chebyshev -o 4 -t bandpass -c 15,35 -r 3 -g
.\design-filter.exe -p chebyshev -o 4 -t bandstop -c 15,35 -r 3 -g
.\design-filter.exe -p chebyshev -o 4 -t lowpass -c 25 -r 3 -g
.\design-filter.exe -p chebyshev -o 4 -t highpass -c 25 -r 3 -g
.\design-filter.exe -p chebyshev-type2 -o 4 -t bandpass -c 15,35 -r 3 -g
.\design-filter.exe -p chebyshev-type2 -o 4 -t bandstop -c 15,35 -r 3 -g
.\design-filter.exe -p chebyshev-type2 -o 4 -t lowpass -c 25 -r 3 -g
.\design-filter.exe -p chebyshev-type2 -o 4 -t highpass -c 25 -r 3 -g
.\design-filter.exe -p brickwall -o 40 -t bandpass -c 15,35 -g
.\design-filter.exe -p brickwall -o 40 -t bandstop -c 15,35 -g
.\design-filter.exe -p brickwall -o 40 -t lowpass -c 25 -g
.\design-filter.exe -p brickwall -o 40 -t highpass -c 25 -g
```

See the [examples for the supported filter types](../README.md#examples).

