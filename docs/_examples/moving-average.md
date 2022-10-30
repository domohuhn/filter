---
layout: example
title: Moving average filters
link: moving-average.html
---

# Moving average filters

Moving average filters compute the average of the last $$k$$ inputs $$x[i]$$, where k is the size of the window of the FIR filter:

$$ y\left[n\right] = \frac{1}{k} \cdot \sum_{i=0}^{k-1} x[n-i] $$

These filters do not have an analog equivalent. They only exist as digital filters. Moving average filters are often used to
filter short-term fluctuations from time series data. This type of filter has often the best response in the time domain, but
the frequency domain will always contain ripples.

## Lowpass filters

Computing the coefficients is really easy for a lowpass, as all coefficients are equal to $$\frac{1}{k}$$.
Here is are graphs of the filter response for a 16 value moving average low pass:

{% assign malp = site.data.examples["malp"] %}
{% include example_display.md filter=malp %}



# Highpass

A 16 value moving average high pass has the following response:

{% assign mahp = site.data.examples["mahp"] %}
{% include example_display.md filter=mahp %}

