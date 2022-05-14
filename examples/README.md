# Examples

## Program "Design Filter"

This a command line program that can be used to design a filter and to generate the example graphs in the doc folder.

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

Example calls producing the outputs in doc:
```
.\design-filter.exe -p moving-average -o 15 -t lowpass -g > movingaverage_lowpass.txt
.\design-filter.exe -p moving-average -o 15 -t highpass -g > movingaverage_highpass.txt
.\design-filter.exe -p exponential -t lowpass -c 15 -g > exponential_lowpass.txt
.\design-filter.exe -p butterworth -o 4 -t bandpass -c 15,35 -g > butterworth_bandpass.txt
.\design-filter.exe -p butterworth -o 4 -t bandstop -c 15,35 -g > butterworth_bandstop.txt
.\design-filter.exe -p butterworth -o 4 -t lowpass -c 25 -g > butterworth_lowpass.txt
.\design-filter.exe -p butterworth -o 4 -t highpass -c 25 -g > butterworth_highpass.txt
.\design-filter.exe -p chebyshev -o 4 -t bandpass -c 15,35 -r 3 -g > chebyshev_bandpass.txt
.\design-filter.exe -p chebyshev -o 4 -t bandstop -c 15,35 -r 3 -g > chebyshev_bandstop.txt
.\design-filter.exe -p chebyshev -o 4 -t lowpass -c 25 -r 3 -g > chebyshev_lowpass.txt
.\design-filter.exe -p chebyshev -o 4 -t highpass -c 25 -r 3 -g > chebyshev_highpass.txt
.\design-filter.exe -p brickwall -o 40 -t bandpass -c 15,35 -g > brickwall_bandpass.txt
.\design-filter.exe -p brickwall -o 40 -t bandstop -c 15,35 -g > brickwall_bandstop.txt
.\design-filter.exe -p brickwall -o 40 -t lowpass -c 25 -g > brickwall_lowpass.txt
.\design-filter.exe -p brickwall -o 40 -t highpass -c 25 -g > brickwall_highpass.txt
```

See 

