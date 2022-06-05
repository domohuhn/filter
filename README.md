# Filter library for C

This repostitory contains a C-library that can be used to both design digital filters, as well as use them.
The library also provides bindings to various other programming languages, like C++ and Javascript via emscripten.

You can try the library online.

The following filter types are supported:

| Name     |      Type     | Characteristics | Examples 
|----------|:-------------:|:-------------:|------------
| Moving average | FIR     | lowpass, highpass | [Examples](doc/moving-average.md)
| exponential    | FIR     | lowpass | [Examples](doc/exponential.md)
| exponential    | IIR     | lowpass | [Examples](doc/exponential.md)
| butterworth    | IIR     | lowpass, highpass, bandpass, bandstop | [Examples](doc/butterworth.md)
| chebyshev (type 1)    | IIR     | lowpass, highpass, bandpass, bandstop | [Examples](doc/chebyshev.md)
| chebyshev (type 2)    | IIR     | lowpass, highpass, bandpass, bandstop |  [Examples](doc/chebyshev.md)
| brickwall    | FIR     | lowpass, highpass, bandpass, bandstop | [Examples](doc/brickwall.md)

The filters are designed using the bilinear z-transform.
In broad terms, you start with the frequency response of the analog filter and compute its poles and zeros. These values are transformed via a bilinear z-transform and expanded into a polynomial. The polynomial is normalized for gain 1 at 0 or the middle of the pass band.
Here is a [Presentation](https://spinlab.wpi.edu/courses/ece503_2014/10-3bilinear_transform.pdf) explaining the process.

[MIT Signal processing continuous and discrete course](https://ocw.mit.edu/courses/2-161-signal-processing-continuous-and-discrete-fall-2008/pages/lecture-notes/)

## Build and run

```bash
cmake -DDH_CFILTER_BUILD_EXAMPLES=ON \
      -DDH_CFILTER_BUILD_TESTS=ON \
      <path to src>
make
# run the unit tests
./test-filter
# use the example program to design a chebyshev bandstop
# -o: order 4
# -c: cutoff 15 hz and 35 Hz
# -s: sampling rate 100 Hz
# -r: ripple 3 dB
# -g: generate graphs
./design-filter -p chebyshev -t bandstop -o 4 -c 15,35 -s 100 -r -3 -g 
```

## Documentation

The [API Documentation](https://domohuhn.github.io/filter/api/index.html) is generated with doxygen and can read online on the github pages.

## License
All code in this repository is licensed with the MIT license, see file "LICENSE".

## Dependencies

The library itself has no dependencies. However the tests, examples and documentation have the following dependencies:

 - [Catch 2](https://github.com/catchorg/Catch2) is used for the test cases.
 - [cxxopts](https://github.com/jarro2783/cxxopts) parses the command line options in the examples.
 - [Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css) for the css styles of the api documentation.
 - [Chart.js](https://www.chartjs.org/) to render the graphs in the documentation.

## Issues

Please report issues and bugs at the issue tracker.
