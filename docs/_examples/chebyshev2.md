---
layout: example
title: Chebyshev Type II filters
link: chebyshev2.html
---

# Chebyshev Type II filters

Here are some examples for chebyshev type 2 filters.

# Lowpass

{% assign cheblp = site.data.examples["cheb2lp"] %}
{% include example_display.md filter=cheblp %}


# Highpass

{% assign chebhp = site.data.examples["cheb2hp"] %}
{% include example_display.md filter=chebhp %}

# Bandpass

{% assign chebbp = site.data.examples["cheb2bp"] %}
{% include example_display.md filter=chebbp %}

# Bandstop

{% assign chebbs = site.data.examples["cheb2bs"] %}
{% include example_display.md filter=chebbs %}
