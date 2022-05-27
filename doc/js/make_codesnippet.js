
function convert_type_to_enum(type){
    switch(type){
        case 0: return "DH_NO_FILTER";
        case 1: return "DH_FIR_MOVING_AVERAGE_LOWPASS";
        case 2: return "DH_FIR_MOVING_AVERAGE_HIGHPASS";
        case 3: return "DH_FIR_EXPONENTIAL_MOVING_AVERAGE_LOWPASS";
        case 4: return "DH_FIR_BRICKWALL_LOWPASS";
        case 5: return "DH_FIR_BRICKWALL_HIGHPASS";
        case 6: return "DH_FIR_BRICKWALL_BANDPASS";
        case 7: return "DH_FIR_BRICKWALL_BANDSTOP";
        case 8: return "DH_IIR_EXPONENTIAL_LOWPASS";
        case 9: return "DH_IIR_BUTTERWORTH_LOWPASS";
        case 10: return "DH_IIR_BUTTERWORTH_HIGHPASS";
        case 11: return "DH_IIR_BUTTERWORTH_BANDPASS";
        case 12: return "DH_IIR_BUTTERWORTH_BANDSTOP";
        case 13: return "DH_IIR_CHEBYSHEV_LOWPASS";
        case 14: return "DH_IIR_CHEBYSHEV_HIGHPASS";
        case 15: return "DH_IIR_CHEBYSHEV_BANDPASS";
        case 16: return "DH_IIR_CHEBYSHEV_BANDSTOP";
        default: return "THERE WAS AN ERROR";
    }
}

function generate_c_lib(options) {
    return `<pre class="code">
<span class="preprocessor">#include</span> <span class="string">"dh/filter.h"</span>

<span class="comment">// Generates a filter designed on the webpage of the
// dh filter library.
// This code was generated from your inputs and is your property.
// USE THIS CODE AT YOUR OWN RISK! THERE IS NO GUARANTEE THAT IT WILL WORK!
// The method initializes the designed filter using the dh filter library:
// https://github.com/domohuhn/filter
// Returns 0 on success, -1 on error.</span>
<span class="keyword">inline</span> <span class="type">int</span> initialize_filter(<span class="type">dh_filter_data</span>* filter_data) {
    <span class="type">dh_filter_parameters</span> opts;
    opts.filter_type = <span class="enum">${convert_type_to_enum(options.filterType.value)}</span>;
    opts.cutoff_frequency_low = <span class="number">${options.cutoffFrequencyLow}</span>;
    opts.cutoff_frequency_high = <span class="number">${options.cutoffFrequencyHigh}</span>;
    opts.sampling_frequency = <span class="number">${options.samplingFrequency}</span>;
    opts.ripple = <span class="number">${options.ripple}</span>;
    opts.filter_order = <span class="number">${options.filterOrder}</span>;
    <span class="keyword">if</span> ( dh_create_filter(filter_data,&amp;opts) != <span class="enum">DH_FILTER_OK</span>) {
        <span class="keyword">return</span> <span class="number">-1</span>;
    }
    <span class="keyword">return</span> <span class="number">0</span>;
}
</pre>`;
}


function generate_cpp_lib(options) {
    return `<pre class="code">
<span class="preprocessor">#include</span> <span class="string">"dh/filter.hpp"</span>

<span class="comment">// Generates a filter designed on the webpage of the
// dh filter library.
// This code was generated from your inputs and is your property.
// USE THIS CODE AT YOUR OWN RISK! THERE IS NO GUARANTEE THAT IT WILL WORK!
// The method initializes the designed filter using the dh filter library:
// https://github.com/domohuhn/filter
// Returns the instantiated filter or throws an exception on error.</span>
<span class="keyword">inline</span> <span class="type">dh::filter</span> initialize_filter() {
    <span class="type">dh_filter_parameters</span> opts;
    opts.filter_type = <span class="enum">${convert_type_to_enum(options.filterType.value)}</span>;
    opts.cutoff_frequency_low = <span class="number">${options.cutoffFrequencyLow}</span>;
    opts.cutoff_frequency_high = <span class="number">${options.cutoffFrequencyHigh}</span>;
    opts.sampling_frequency = <span class="number">${options.samplingFrequency}</span>;
    opts.ripple = <span class="number">${options.ripple}</span>;
    opts.filter_order = <span class="number">${options.filterOrder}</span>;
    <span class="keyword">return</span> <span class="type">dh::filter</span>(opts);
}
</pre>`;
}


function generate_javascript_lib(options) {
    var enu = convert_type_to_enum(options.filterType.value);
    return `<pre class="code">
<span class="comment">// Generates a filter designed on the webpage of the
// dh filter library.
// This code was generated from your inputs and is your property.
// USE THIS CODE AT YOUR OWN RISK! THERE IS NO GUARANTEE THAT IT WILL WORK!
// The method initializes the designed filter using the dh filter library with the emscripten bindings:
// https://github.com/domohuhn/filter
// Returns 0 on success, -1 on error.</span>
<span class="keyword">var</span> parameters = <span class="keyword">new</span> Module.FilterParameters();
parameters.filterType = <span class="enum">Module.FilterType.${enu.substring(3)}</span>;
parameters.cutoffFrequencyLow = <span class="number">${options.cutoffFrequencyLow}</span>;
parameters.cutoffFrequencyHigh = <span class="number">${options.cutoffFrequencyHigh}</span>;
parameters.samplingFrequency = <span class="number">${options.samplingFrequency}</span>;
parameters.ripple = <span class="number">${options.ripple}</span>;
parameters.filterOrder = <span class="number">${options.filterOrder}</span>;
<span class="keyword">var</span> filter = <span class="keyword">new</span> Module.Filter(pars);
</pre>`;
}


function generate_recurrence_relation(options, filter) {
    var rv = `<pre class="code">
Filter type        : <span class="enum">${convert_type_to_enum(options.filterType.value)}</span>
Order              : <span class="number">${options.filterOrder}</span>
Sampling Frequency : <span class="number">${options.samplingFrequency}</span>
Cutoff (low)       : <span class="number">${options.cutoffFrequencyLow}</span>
Cutoff (high)      : <span class="number">${options.cutoffFrequencyHigh}</span>
Ripple (db)        : <span class="number">${options.ripple}</span>
Gain               : <span class="number">1.0</span>
Recurrence relation:
y[n] =  `
    for(i=0;i<filter.countFeedforwardCoefficients;++i){
        if(i>0) {
            rv +=  `        `;
        }
        rv +=  `+ x[n-${i}] * <span class="number">${filter.feedforwardCoefficient(i)}</span>\n`;
    }
    rv += "\n";
    for(i=1;i<filter.countFeedbackCoefficients;++i){
        rv +=  `        - y[n-${i}] * <span class="number">${filter.feedbackCoefficient(i)}</span>\n`;
    }
rv +=  `
</pre>`;
    return rv;
}



function generate_c(options, filter) {
    var inputcoeffs = '';
    for(i=0;i<filter.countFeedforwardCoefficients;++i) {
        inputcoeffs += ',\n        '+filter.feedforwardCoefficient(i);
    }
    inputcoeffs = inputcoeffs.substring(10);
    var outputcoeffs = '';
    for(i=0;i<filter.countFeedbackCoefficients;++i) {
        outputcoeffs += ',\n        '+filter.feedbackCoefficient(i);
    }
    outputcoeffs = outputcoeffs.substring(10);
    var rv = `<pre class="code">
<span class="preprocessor">#ifndef</span> <span class="enum">GENERATED_C_CODE_INCLUDED</span>
<span class="preprocessor">#define</span> <span class="enum">GENERATED_C_CODE_INCLUDED</span>
    
<span class="preprocessor">#ifdef</span> <span class="enum">__cplusplus</span>
<span class="keyword">extern</span> <span class="string">"C"</span> {
<span class="preprocessor">#endif</span>
    
<span class="preprocessor">#include</span> <span class="string">"stddef.h"</span>
    
<span class="comment">// Generates a filter designed on the webpage of the
// dh filter library.
// This code was generated from your inputs and is your property.
// USE THIS CODE AT YOUR OWN RISK! THERE IS NO GUARANTEE THAT IT WILL WORK!
//
// See:
// https://github.com/domohuhn/filter
// Filter parameters:
// Filter type        : <span class="enum">${convert_type_to_enum(options.filterType.value)}</span>
// Order              : <span class="number">${options.filterOrder}</span>
// Sampling Frequency : <span class="number">${options.samplingFrequency}</span>
// Cutoff (low)       : <span class="number">${options.cutoffFrequencyLow}</span>
// Cutoff (high)      : <span class="number">${options.cutoffFrequencyHigh}</span>
// Ripple (db)        : <span class="number">${options.ripple}</span>
// Gain               : <span class="number">1.0</span>

// Data struct holding the past inputs and outputs. Initialize the struct by setting all values to 0.</span>
<span class="keyword">typedef struct</span> {
    <span class="type">double</span> inputs[${filter.countFeedforwardCoefficients}];
    <span class="type">double</span> outputs[${filter.countFeedbackCoefficients}];
    <span class="type">size_t</span> input_index;
    <span class="type">size_t</span> output_index;
} <span class="type">filter_data</span>;

<span class="comment">// Updates the filter and returns the filtered value</span>
<span class="keyword">inline</span> <span class="type">double</span> update_filter(<span class="type">filter_data</span>* filter, <span class="type">double</span> input) {`
    if(filter.countFeedforwardCoefficients>1) {
        rv += `
    filter-&gt;input_index = filter-&gt;input_index &gt; <span class="number">0</span> ? filter-&gt;input_index - <span class="number">1</span>: <span class="number">${filter.countFeedforwardCoefficients-1}</span>;
    filter-&gt;inputs[filter-&gt;input_index] = input;
    <span class="keyword">const</span> <span class="type">double</span> input_coefficients[]={${inputcoeffs}};
    <span class="type">double</span> out = input * input_coefficients[0];
    <span class="type">size_t</span> split_loops = ${filter.countFeedforwardCoefficients} - filter-&gt;input_index;
    <span class="keyword">for</span>(<span class="type">size_t</span> i=<span class="number">1</span>; i &lt; split_loops; ++i) {
        out += input_coefficients[i] * filter-&gt;inputs[filter-&gt;input_index + i];
    }
    <span class="keyword">for</span>(<span class="type">size_t</span> i=split_loops; i &lt; <span class="number">${filter.countFeedforwardCoefficients}</span>; ++i) {
        out += input_coefficients[i] * filter-&gt;inputs[i-split_loops];
    }`
    } else if (filter.countFeedforwardCoefficients===1) {
        rv += `\n    <span class="type">double</span> out = input * ${filter.feedforwardCoefficient(0)};`;
    }
    if(filter.countFeedbackCoefficients>2) {
        rv += `
    <span class="keyword">const</span> <span class="type">double</span> output_coefficients[]={${outputcoeffs}};
    filter-&gt;output_index = filter-&gt;output_index &gt; <span class="number">0</span> ? filter-&gt;output_index - <span class="number">1</span>: <span class="number">${filter.countFeedbackCoefficients-1}</span>;
    split_loops = ${filter.countFeedbackCoefficients} - filter-&gt;output_index;
    <span class="keyword">for</span>(<span class="type">size_t</span> i=<span class="number">1</span>; i &lt; split_loops; ++i) {
        out -= output_coefficients[i] * filter-&gt;outputs[filter-&gt;output_index + i];
    }
    <span class="keyword">for</span>(<span class="type">size_t</span> i=split_loops; i &lt; <span class="number">${filter.countFeedbackCoefficients}</span>; ++i) {
        out -= output_coefficients[i] * filter-&gt;outputs[i-split_loops];
    }
    filter-&gt;outputs[filter-&gt;output_index] = out;`
    } else if(filter.countFeedbackCoefficients===2) {
        rv += `
    out += ${-filter.feedbackCoefficient(1)} * filter-&gt;outputs[0];
    filter-&gt;outputs[0] = out;`;
    }
    rv += `
    <span class="keyword">return</span> out;
}

<span class="preprocessor">#ifdef</span> <span class="enum">__cplusplus</span>
}
<span class="preprocessor">#endif</span>

<span class="preprocessor">#endif</span> <span class="comment">/* GENERATED_C_CODE_INCLUDED */</span>

</pre>`
    return rv;
}

