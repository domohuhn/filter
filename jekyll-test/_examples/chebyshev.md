---
layout: example
title: Chebyshev Type I filters
link: /examples/chebyshev.html
---

# Chebyshev Type I filters

Here are some examples of chebyshev filters.

# Lowpass

{% assign cheblp = site.data.examples["cheblp"] %}
{% include example_display.md filter=cheblp %}


# Highpass

{% assign chebhp = site.data.examples["chebhp"] %}
{% include example_display.md filter=chebhp %}

# Bandpass

{% assign chebbp = site.data.examples["chebbp"] %}
{% include example_display.md filter=chebbp %}

# Bandstop

{% assign chebbs = site.data.examples["chebbs"] %}
{% include example_display.md filter=chebbs %}
