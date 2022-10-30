---
layout: example
title: Exponential filters
link: exponential.html
---

# Exponential filters

The output $$y[n]$$ of exponential filters is characterised by an exponential decay of the output if the new inputs $$x[n]$$ are set to $$0$$. These filters are IIR filters with order 1.

$$ y\left[n\right] = p \cdot x[n] + \left( 1 - p \right) \cdot y[n-1] $$

The value of $$p$$ must be in the range $$ p \in \left[0, 1\right]$$ for a stable filter.

# Lowpass

For example, here is an IIR exponential low pass:

{% assign explp = site.data.examples["explp"] %}
{% include example_display.md filter=explp %}

Any IIR filter can also be approximated as finite impulse response filter. 
The conversion can always be done with these steps:

   - Design the IIR filter.
   - Use the Filter to filter the impulse response in a given window lenght.
   - The outputs of the IIR filter are the coefficients of a FIR filter with the given window size.

You may have to expirement a bit to find the correct window size for your needs. 
Smaller window sizes will distort the response of the filter, but reduce the
complexity (or computation time). When you design your filter, you must take that into consideration
to get a filter that works for your needs.
In this case, a window size of 32 was chosen.

{% assign expfirlp = site.data.examples["expfirlp"] %}
{% include example_display.md filter=expfirlp %}
