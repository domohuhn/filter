---
layout: example
title: Brickwall filters
link: brickwall.html
---

# Brickwall filters

Here are some examples of brickwall filters. These filters are finite impulse response filters. The frequency response will converge to a step function for increasing window sizes. However, these filters have a large delay of half the window size.

# Lowpass

{% assign filtlp = site.data.examples["brilp"] %}
{% include example_display.md filter=filtlp %}

# Highpass

{% assign filthp = site.data.examples["brihp"] %}
{% include example_display.md filter=filthp %}

# Bandpass

{% assign filtbp = site.data.examples["bribp"] %}
{% include example_display.md filter=filtbp %}

# Bandstop

{% assign filtbs = site.data.examples["bribs"] %}
{% include example_display.md filter=filtbs %}
