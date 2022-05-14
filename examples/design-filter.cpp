
#include "cxxopts.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "dh/filter.h"
#include "dh/utility.h"
#include <stdexcept>
#include <iomanip>


/** Converts the command line options. May throw on error. */
dh_filter_options convert_options(cxxopts::ParseResult& result);


void print_parameters(dh_filter_options opts)
{
    dh_filter_data filter_data{};
    int status = dh_create_filter(&filter_data,&opts);
    if(status != DH_FILTER_OK) {
        throw std::runtime_error("Failed to construct filter!");
    }
    std::cout<<"# Feedforward parameters : "<<filter_data.number_coefficients_in<<" (multiplied with inputs x)\n";
    std::cout<<"# Feedback parameters : "<<filter_data.number_coefficients_out<<" (multiplied with outputs y)\n";
    std::cout<<"# Gain: 1\n";
    std::cout<<"# Recurrence relation:\n";
    std::cout<<"# y[n] = "<<"\n";
    for(size_t i=0; i<filter_data.number_coefficients_in; ++i) {
        std::cout<<"#     + (x[n-"<<std::setw(3)<< i << "] * "<<std::setprecision(15)<< filter_data.coefficients_in[i] << ")\n";
    }
    std::cout<<"#\n";
    for(size_t i=1; i<filter_data.number_coefficients_out; ++i) {
        std::cout<<"#     - (y[n-"<<std::setw(3)<< i << "] * "<<std::setprecision(15)<< filter_data.coefficients_out[i] << ")\n";
    }
    dh_free_filter(&filter_data);
}

/** This function creates a filter, then filters an impulse input and returns the response of the filter. */
std::vector<std::pair<double,double>> impulse_response(dh_filter_options opts) {
    dh_filter_data filter_data{};
    int status = dh_create_filter(&filter_data,&opts);
    if(status != DH_FILTER_OK) {
        throw std::runtime_error("Failed to construct filter!");
    }
    std::vector<std::pair<double,double>> rv;
    double input = 0.0;
    double output = 0.0;
    for(size_t i=0; i<=100; ++i) {
        if(i==10) {
            input = 100.0;
        } else {
            input = 0.0;
        }
        if(dh_filter(&filter_data, input,&output) != DH_FILTER_OK){
            throw std::runtime_error("Error while filtering impulse response!");
        }
        rv.emplace_back(std::make_pair(input,output));
    }
    dh_free_filter(&filter_data);
    return rv;
}

/** This function creates a filter, then filters a step function input and returns the response of the filter. */
std::vector<std::pair<double,double>> step_response(dh_filter_options opts) {
    dh_filter_data filter_data{};
    int status = dh_create_filter(&filter_data,&opts);
    if(status != DH_FILTER_OK) {
        throw std::runtime_error("Failed to construct filter!");
    }
    std::vector<std::pair<double,double>> rv;
    double input = 0.0;
    double output = 0.0;
    for(size_t i=0; i<=100; ++i) {
        if(i>9) {
            input = 100.0;
        } else {
            input = 0.0;
        }
        if(dh_filter(&filter_data, input,&output) != DH_FILTER_OK){
            throw std::runtime_error("Error while filtering step response!");
        }
        rv.emplace_back(std::make_pair(input,output));
    }
    dh_free_filter(&filter_data);
    return rv;
}


/** This function creates a filter, then filters a sine wave with a given frequency and returns
 * the filtered amplitude in the steady state (after 1000 filter cycles). */
std::pair<double,double> gain(dh_filter_options opts, double fraction) {
    dh_filter_data filter_data{};
    int status = dh_create_filter(&filter_data,&opts);
    if(status != DH_FILTER_OK) {
        throw std::runtime_error("Failed to construct filter!");
    }
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    double amp = 100.0;
    double input = 0.0;
    double output = 0.0;
    double factor = 2*M_PI*fraction;
    for(size_t i=0; i<=10000; ++i) {
        input = amp * std::sin(factor * i);
        if(factor==0.0){
            input=amp;
        }
        if(dh_filter(&filter_data, input,&output) != DH_FILTER_OK){
            throw std::runtime_error("Error while filtering step response!");
        }
        // only measure amplitude in steady state
        if(i<1000) {
            continue;
        }
        if(output<min){
            min = output;
        }
        if(output>max){
            max = output;
        }
    }
    dh_free_filter(&filter_data);
    if(fraction>0.0) {
        return std::make_pair(fraction, (max-min)/(2*amp));
    }
    else {
        return std::make_pair(fraction, output/amp);
    }
}

/** This function creates the freuency response by filtering sine waves with different frequencies. */
std::vector<std::pair<double,double>> frequency_response(dh_filter_options opts)
{
    std::vector<std::pair<double,double>> rv;
    for(size_t i=0;i<=100;i++) {
        rv.emplace_back(gain(opts,i/200.0));
    }
    return rv;
}

/** This function computes the phase shift from the coefficients of the filters. */
std::vector<std::pair<double,double>> phase_shift(dh_filter_options opts)
{
    dh_filter_data filter_data{};
    int status = dh_create_filter(&filter_data,&opts);
    if(status != DH_FILTER_OK) {
        throw std::runtime_error("Failed to construct filter!");
    }
    std::vector<std::pair<double,double>> rv;
    for(size_t i=0;i<=100;i++) {
        auto comp_gain = dh_gain_at(filter_data.coefficients_in,filter_data.number_coefficients_in,filter_data.coefficients_out,filter_data.number_coefficients_out, i/200.0);
        auto shift = atan2(cimag(comp_gain),creal(comp_gain)) / M_PI * 180.0;
        rv.emplace_back(std::make_pair(i/200.0,shift));
    }
    dh_free_filter(&filter_data);
    return rv;
}

void create_graphs(dh_filter_options opts) {
    auto impulse = impulse_response(opts);
    auto step = step_response(opts);
    auto freq = frequency_response(opts);
    auto phase = phase_shift(opts);
    std::cout << "#cycle    input(impulse)    output(impulse)    input(step)    output(step)    frequency/sampling_frequency    gain    phase(degree)\n";
    for(int i=0;i<=100;++i) {
        std::cout <<std::setw(3)<<i-10<<"    "<<std::setw(5)<<impulse[i].first<<"    "<<std::setw(22)<<impulse[i].second<<"    "
        <<std::setw(5)<<step[i].first<<"    "<<std::setw(22)<<step[i].second<<"    "
        <<std::setw(5)<< freq[i].first <<"    "<<std::setw(22)<< freq[i].second<<"    "<<std::setw(22)<< phase[i].second <<"\n";
    }
}

int main(int argc, const char * argv[]) {
    cxxopts::Options options("design-filter", "A program that can be used to design FIR and IIR filters.");

    options.add_options()
        ("p,parameter-type", "Filter type (one of no-filter, butterworth, chebyshev, moving-average, brickwall, exponential)", cxxopts::value<std::string>()->default_value("butterworth"))
        ("t,type", "Filter type (one of lowpass, highpass, bandpass, bandstop)", cxxopts::value<std::string>()->default_value("lowpass"))
        ("o,order", "Order of the filter", cxxopts::value<int>()->default_value("4"))
        ("g,graphs", "Create data for graphs", cxxopts::value<bool>()->default_value("false"))
        ("s,sampling-frequency", "Sampling frequency in Hz", cxxopts::value<double>()->default_value("100"))
        ("c,cutoff-frequency", "Cutoff frequencies in Hz", cxxopts::value<std::vector<double>>()->default_value("10,20"))
        ("r,ripple", "Ripple in dB for chebyshev", cxxopts::value<double>()->default_value("3.0"))
        ("h,help", "Print usage")
    ;

    try {
        auto result = options.parse(argc, argv);
        if (result.count("help"))
        {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }
        auto opts = convert_options(result);
        print_parameters(opts);
        if(result["graphs"].as<bool>()) {
            create_graphs(opts);
        }
    }
    catch (const cxxopts::OptionException &e) {
        std::cout << "error parsing options: " << e.what() << std::endl << std::endl;
        std::cout << options.help({""}) << std::endl;
        exit(1);
    }
    catch (const std::exception& e) {
        std::cout << "Runtime error: " << e.what() << std::endl << std::endl;
        std::cout << options.help({""}) << std::endl;
        exit(1);
    }
    catch (...) {
        std::cout << "Unhandled error." << std::endl << std::endl;
        std::cout << options.help({""}) << std::endl;
        exit(1);
    }
    return 0;
}


DH_FILTER_TYPE select_type(cxxopts::ParseResult& result, DH_FILTER_TYPE type){
    auto param = result["type"].as<std::string>();
    std::cout<< "# Filter type        : '"<<param<<"'\n";
    if (type == DH_FIR_MOVING_AVERAGE) {
        if(param=="lowpass") {
            return type;
        } else if(param=="highpass") {
            return DH_FIR_MOVING_AVERAGE_HIGHPASS;
        } else {
            throw std::runtime_error("Unknown filter type! Only high- and lowpass is supported for moving average filters!");
        }
    }
    else if(type == DH_IIR_EXPONENTIAL_LOWPASS) {
        if(param=="lowpass") {
            return type;
        } else {
            throw std::runtime_error("Unknown filter type! Only lowpass is supported for exponential filters!");
        }
    }
    else if(param=="lowpass"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_LOWPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_LOWPASS;
        }
        return DH_IIR_BUTTERWORTH_LOWPASS;
    }
    else if(param=="highpass"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_HIGHPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_HIGHPASS;
        }
        return DH_IIR_BUTTERWORTH_HIGHPASS;
    }
    else if(param=="bandpass"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_BANDPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_BANDPASS;
        }
        return DH_IIR_BUTTERWORTH_BANDPASS;
    }
    else if(param=="bandstop"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_BANDSTOP;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_BANDSTOP;
        }
        return DH_IIR_BUTTERWORTH_BANDSTOP;
    }
    else {
        throw std::runtime_error("Unknown filter type! Only butterworth, brickwall, chebyshev or moving-average are supported for option -p.\nParsed: '"+param +"'");
    }
    return type;
}

dh_filter_options convert_options(cxxopts::ParseResult& result) 
{
    dh_filter_options options{};
    auto param = result["parameter-type"].as<std::string>();
    int order = result["order"].as<int>();
    double sampling_frequency_hz = result["sampling-frequency"].as<double>();
    auto cutoffs_hz = result["cutoff-frequency"].as<std::vector<double>>();
    if (order<0) {
        throw std::runtime_error("Order of the filter must be positive! \nParsed: '-o "+std::to_string(order)+"'");
    }
    std::cout<< "# Parameter type          : '"<<param<<"'\n";
    if (param == "butterworth") {
        std::cout<< "# Order                   : "<<order<<"\n";
        options.filter_type = select_type(result ,DH_IIR_BUTTERWORTH_LOWPASS);
        options.parameters.butterworth.filter_order = order;
        options.parameters.butterworth.sampling_frequency_hz = sampling_frequency_hz;
        
        std::cout<< "# Sampling frequency : "<<options.parameters.butterworth.sampling_frequency_hz<<" Hz\n";
        if(cutoffs_hz.size()>0) {
            options.parameters.butterworth.cutoff_frequency_hz = cutoffs_hz[0];
            std::cout<< "# Cutoff 1           : "<<options.parameters.butterworth.cutoff_frequency_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()>1 && (options.filter_type==DH_IIR_BUTTERWORTH_BANDPASS || options.filter_type==DH_IIR_BUTTERWORTH_BANDSTOP)){
            options.parameters.butterworth.cutoff_frequency_2_hz = cutoffs_hz[1];
            std::cout<< "# Cutoff 2           : "<<options.parameters.butterworth.cutoff_frequency_2_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()==0 || (cutoffs_hz.size()<2 && (options.filter_type==DH_IIR_BUTTERWORTH_BANDPASS || options.filter_type==DH_IIR_BUTTERWORTH_BANDSTOP))) {
            throw std::runtime_error("You must provide cutoff frequencies in Hz via -c\nExample '-c 10' for lowpass, or '-c 10,20' a bandpass or bandstop");
        }
    }
    else if (param == "brickwall") {
        std::cout<< "# Order                   : "<<order<<"\n";
        options.filter_type = select_type(result ,DH_FIR_BRICKWALL_LOWPASS);
        options.parameters.brickwall.filter_order = order;
        options.parameters.brickwall.sampling_frequency_hz = sampling_frequency_hz;
        
        std::cout<< "# Sampling frequency : "<<options.parameters.brickwall.sampling_frequency_hz<<" Hz\n";
        if(cutoffs_hz.size()>0) {
            options.parameters.brickwall.cutoff_frequency_hz = cutoffs_hz[0];
            std::cout<< "# Cutoff 1           : "<<options.parameters.brickwall.cutoff_frequency_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()>1 && (options.filter_type==DH_FIR_BRICKWALL_BANDPASS || options.filter_type==DH_FIR_BRICKWALL_BANDSTOP)){
            options.parameters.brickwall.cutoff_frequency_2_hz = cutoffs_hz[1];
            std::cout<< "# Cutoff 2           : "<<options.parameters.brickwall.cutoff_frequency_2_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()==0 || (cutoffs_hz.size()<2 && (options.filter_type==DH_FIR_BRICKWALL_BANDPASS || options.filter_type==DH_FIR_BRICKWALL_BANDSTOP))) {
            throw std::runtime_error("You must provide cutoff frequencies in Hz via -c\nExample '-c 10' for lowpass, or '-c 10,20' a bandpass or bandstop");
        }
    }
    else if (param == "chebyshev") {
        std::cout<< "# Order                   : "<<order<<"\n";
        options.filter_type = select_type(result ,DH_IIR_CHEBYSHEV_LOWPASS);
        options.parameters.chebyshev.filter_order = order;
        options.parameters.chebyshev.sampling_frequency_hz = sampling_frequency_hz;

        std::cout<< "# Sampling frequency : "<<options.parameters.chebyshev.sampling_frequency_hz<<" Hz\n";
        if(cutoffs_hz.size()>0) {
            options.parameters.chebyshev.cutoff_frequency_hz = cutoffs_hz[0];
            std::cout<< "# Cutoff 1           : "<<options.parameters.chebyshev.cutoff_frequency_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()>1 && (options.filter_type==DH_IIR_CHEBYSHEV_BANDPASS || options.filter_type==DH_IIR_CHEBYSHEV_BANDSTOP)){
            options.parameters.chebyshev.cutoff_frequency_2_hz = cutoffs_hz[1];
            std::cout<< "# Cutoff 2           : "<<options.parameters.chebyshev.cutoff_frequency_2_hz<<" Hz\n";
        }
        if(cutoffs_hz.size()==0 || (cutoffs_hz.size()<2 && (options.filter_type==DH_IIR_CHEBYSHEV_BANDPASS || options.filter_type==DH_IIR_CHEBYSHEV_BANDSTOP))) {
            throw std::runtime_error("You must provide cutoff frequencies in Hz via -c\nExample '-c 10' for lowpass, or '-c 10,20' a bandpass or bandstop");
        }
        options.parameters.chebyshev.ripple = -std::abs(result["ripple"].as<double>());
        std::cout<< "# Ripple             : "<<options.parameters.chebyshev.ripple<<" dB\n";
    }
    else if (param == "moving-average") {
        std::cout<< "# Order                   : "<<order<<"\n";
        options.filter_type = select_type(result ,DH_FIR_MOVING_AVERAGE);
        options.parameters.moving_average.filter_order = order;
    }
    else if (param == "exponential") {
        options.filter_type =  select_type(result ,DH_IIR_EXPONENTIAL_LOWPASS);
        options.parameters.exponential.filter_order = 1;
        std::cout<< "# Sampling frequency : "<<sampling_frequency_hz<<" Hz\n";
        if(cutoffs_hz.size()>0) {
            options.parameters.exponential.alpha =  cutoffs_hz[0]/sampling_frequency_hz ;
            std::cout<< "# Cutoff 1           : "<<cutoffs_hz[0]<<" Hz\n";
        }
        else {
            throw std::runtime_error("You must provide cutoff frequencies in Hz via -c\nExample '-c 10' for lowpass, or '-c 10,20' a bandpass or bandstop");
        }
    }
    else if (param == "no-filter") {
        options.filter_type = DH_FIR_MOVING_AVERAGE;
        options.parameters.moving_average.filter_order = 0;
    }
    else {
        throw std::runtime_error("Unknown filter parametrization! Only no-filter, butterworth, chebyshev, brickwall, exponential, or moving-average are supported for option -p.\nParsed: '"+param +"'");
    }
    return options;
}

