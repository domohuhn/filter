
#include "cxxopts.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "dh/cpp/filter.hpp"
#include <stdexcept>
#include <iomanip>


/** Converts the command line options. May throw on error. */
dh_filter_parameters convert_options(cxxopts::ParseResult& result);


void print_parameters(dh_filter_parameters opts)
{
    auto filter_data = dh::filter(opts);
    auto feedforward = filter_data.feedforward_coefficients();
    auto feedback = filter_data.feedback_coefficients();
    std::cout<<"# Feedforward parameters : "<<feedforward.size()<<" (multiplied with inputs x)\n";
    std::cout<<"# Feedback parameters : "<<feedback.size()<<" (multiplied with outputs y)\n";
    std::cout<<"# Gain: 1\n";
    std::cout<<"# Recurrence relation:\n";
    std::cout<<"# y[n] = "<<"\n";
    for(size_t i=0; i<feedforward.size(); ++i) {
        std::cout<<"#     + (x[n-"<<std::setw(3)<< i << "] * "<<std::setprecision(15)<< feedforward[i] << ")\n";
    }
    std::cout<<"#\n";
    for(size_t i=1; i<feedback.size(); ++i) {
        std::cout<<"#     - (y[n-"<<std::setw(3)<< i << "] * "<<std::setprecision(15)<< feedback[i] << ")\n";
    }
}

void create_graphs(dh_filter_parameters opts) {
    auto filter = dh::filter(opts);
    auto impulse = filter.compute_impulse_response();
    auto step = filter.compute_step_response();
    auto freq = filter.compute_frequency_response(impulse.size());
    std::cout << "#cycle    input(impulse)    output(impulse)    input(step)    output(step)    frequency/sampling_frequency    gain    phase(degree)\n";
    for(size_t i=0;i<=impulse.size();++i) {
        int si = i;
        std::cout <<std::setw(3)<<si-10<<"    "<<std::setw(5)<<impulse[i].input<<"    "<<std::setw(22)<<impulse[i].output<<"    "
        <<std::setw(5)<<step[i].input<<"    "<<std::setw(22)<<step[i].output<<"    "
        <<std::setw(5)<< freq[i].frequency <<"    "<<std::setw(22)<< freq[i].gain<<"    "<<std::setw(22)<< freq[i].phase_shift <<"\n";
    }
}

int main(int argc, const char * argv[]) {
    cxxopts::Options options("design-filter", "A program that can be used to design FIR and IIR filters.");

    options.add_options()
        ("p,parameter-type", "Filter type (one of no-filter, butterworth, chebyshev, chebyshev-type2, moving-average, brickwall, exponential)", cxxopts::value<std::string>()->default_value("butterworth"))
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
    if (type == DH_FIR_MOVING_AVERAGE_LOWPASS) {
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
        } else if( type == DH_IIR_CHEBYSHEV2_LOWPASS ) {
            return DH_IIR_CHEBYSHEV2_LOWPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_LOWPASS;
        }
        return type;
    }
    else if(param=="highpass"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_HIGHPASS;
        } else if( type == DH_IIR_CHEBYSHEV2_LOWPASS ) {
            return DH_IIR_CHEBYSHEV2_HIGHPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_HIGHPASS;
        }
        return DH_IIR_BUTTERWORTH_HIGHPASS;
    }
    else if(param=="bandpass"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_BANDPASS;
        } else if( type == DH_IIR_CHEBYSHEV2_LOWPASS ) {
            return DH_IIR_CHEBYSHEV2_BANDPASS;
        } else if (type == DH_FIR_BRICKWALL_LOWPASS) {
            return DH_FIR_BRICKWALL_BANDPASS;
        }
        return DH_IIR_BUTTERWORTH_BANDPASS;
    }
    else if(param=="bandstop"){
        if( type == DH_IIR_CHEBYSHEV_LOWPASS ) {
            return DH_IIR_CHEBYSHEV_BANDSTOP;
        } else if( type == DH_IIR_CHEBYSHEV2_LOWPASS ) {
            return DH_IIR_CHEBYSHEV2_BANDSTOP;
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

dh_filter_parameters convert_options(cxxopts::ParseResult& result) 
{
    dh_filter_parameters options{};
    auto param = result["parameter-type"].as<std::string>();
    int order = result["order"].as<int>();
    double sampling_frequency_hz = result["sampling-frequency"].as<double>();
    auto cutoffs_hz = result["cutoff-frequency"].as<std::vector<double>>();
    if (order<0) {
        throw std::runtime_error("Order of the filter must be positive! \nParsed: '-o "+std::to_string(order)+"'");
    }
    if (sampling_frequency_hz<0) {
        throw std::runtime_error("Sampling frequency must be positive! \nParsed: '-s "+std::to_string(sampling_frequency_hz)+"'");
    }
    std::cout<< "# Parameter type          : '"<<param<<"'\n";
    std::cout<< "# Order                   : "<<order<<"\n";
    options.sampling_frequency = sampling_frequency_hz;
    std::cout<< "# Sampling frequency : "<<options.sampling_frequency<<" Hz\n";
    options.filter_order = order;

    bool needsCutoff1 = false;
    bool needsCutoff2 = false;

    if (param == "butterworth") {
        options.filter_type = select_type(result ,DH_IIR_BUTTERWORTH_LOWPASS);
        needsCutoff1 = true;
        needsCutoff2 = (options.filter_type==DH_IIR_BUTTERWORTH_BANDPASS || options.filter_type==DH_IIR_BUTTERWORTH_BANDSTOP);
    }
    else if (param == "brickwall") {
        options.filter_type = select_type(result ,DH_FIR_BRICKWALL_LOWPASS);
        needsCutoff1 = true;
        needsCutoff2 = (options.filter_type==DH_FIR_BRICKWALL_BANDPASS || options.filter_type==DH_FIR_BRICKWALL_BANDSTOP);
    }
    else if (param == "chebyshev") {
        options.filter_type = select_type(result ,DH_IIR_CHEBYSHEV_LOWPASS);
        needsCutoff1 = true;
        needsCutoff2 = (options.filter_type==DH_IIR_CHEBYSHEV_BANDPASS || options.filter_type==DH_IIR_CHEBYSHEV_BANDSTOP);
        options.ripple = -std::abs(result["ripple"].as<double>());
        std::cout<< "# Ripple             : "<<options.ripple<<" dB\n";
    }
    else if (param == "chebyshev-type2") {
        options.filter_type = select_type(result ,DH_IIR_CHEBYSHEV2_LOWPASS);
        needsCutoff1 = true;
        needsCutoff2 = (options.filter_type==DH_IIR_CHEBYSHEV2_BANDPASS || options.filter_type==DH_IIR_CHEBYSHEV2_BANDSTOP);
        options.ripple = -std::abs(result["ripple"].as<double>());
        std::cout<< "# Ripple             : "<<options.ripple<<" dB\n";
    }
    else if (param == "moving-average") {
        options.filter_type = select_type(result ,DH_FIR_MOVING_AVERAGE_LOWPASS);
    }
    else if (param == "exponential") {
        options.filter_type =  select_type(result ,DH_IIR_EXPONENTIAL_LOWPASS);
        needsCutoff1 = true;
    }
    else if (param == "no-filter") {
        options.filter_type = DH_NO_FILTER;
        options.filter_order = 0;
    }
    else {
        throw std::runtime_error("Unknown filter parametrization! Only no-filter, butterworth, chebyshev, brickwall, exponential, or moving-average are supported for option -p.\nParsed: '"+param +"'");
    }
    if(cutoffs_hz.size()>0 && needsCutoff1) {
        options.cutoff_frequency_low = cutoffs_hz[0];
        std::cout<< "# Cutoff 1           : "<<options.cutoff_frequency_low<<" Hz\n";
    }
    if(cutoffs_hz.size()>1 && needsCutoff2){
        options.cutoff_frequency_high = cutoffs_hz[1];
        std::cout<< "# Cutoff 2           : "<<options.cutoff_frequency_high<<" Hz\n";
    }
    if((needsCutoff1&&cutoffs_hz.size()==0) || (cutoffs_hz.size()<2 && needsCutoff2)) {
        throw std::runtime_error("You must provide cutoff frequencies in Hz via -c\nExample '-c 10' for lowpass, or '-c 10,20' a bandpass or bandstop");
    }
    return options;
}

