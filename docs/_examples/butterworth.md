---
layout: example
title: Butterworth filters
link: /examples/butterworth.html
---

# Butterworth filters

Here are some examples of butterworth filters. See [Wikipedia](https://en.wikipedia.org/wiki/Butterworth_filter).

# Lowpass

{% assign filtlp = site.data.examples["butlp"] %}
{% include example_display.md filter=filtlp %}

# Highpass

{% assign filthp = site.data.examples["buthp"] %}
{% include example_display.md filter=filthp %}

# Bandpass

{% assign filtbp = site.data.examples["butbp"] %}
{% include example_display.md filter=filtbp %}

# Bandstop

{% assign filtbs = site.data.examples["butbs"] %}
{% include example_display.md filter=filtbs %}
