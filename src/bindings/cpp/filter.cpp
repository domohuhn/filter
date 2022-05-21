#include "cpp/filter.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
#include "dh/filter.h"

namespace dh {


filter::filter(dh_filter_parameters options) : options_(options) {
    create_internal_data();
}

filter::~filter() {
    dh_free_filter(&data_);
}

void filter::create_internal_data() {
    auto status = dh_create_filter(&data_,&options_);
    switch (status)
    {
    case DH_FILTER_OK:
        break;
    case DH_FILTER_UNKNOWN_FILTER_TYPE:
        throw error("Unkown filter type");
    case DH_FILTER_ALLOCATION_FAILED:
        throw error("Allocation of filter failed");
    default:
        throw error("Unspecified error");
    }
}
void filter::deep_copy(const dh_filter_data& other) {
    std::copy(other.inputs, other.inputs+data_.number_coefficients_in, data_.inputs);
    std::copy(other.outputs, other.outputs+data_.number_coefficients_out, data_.outputs);
    data_.current_input_index = other.current_input_index;
    data_.current_output_index = other.current_output_index;
    data_.initialized = other.initialized;
    data_.current_value = other.current_value;
}

filter::filter(const filter& other) : filter(other.options_) {
    deep_copy(other.data_);
}

filter& filter::operator=(const filter& other) {
    dh_free_filter(&data_);
    options_ = other.options_;
    create_internal_data();
    deep_copy(other.data_);
    return *this;
}

filter::filter(filter&& other) : options_(other.options_), data_(other.data_) {
    other.data_.buffer_needs_cleanup = false;
    dh_free_filter(&other.data_);
}

filter& filter::operator=(filter&& other) {
    dh_free_filter(&data_);
    options_ = other.options_;
    data_ = other.data_;
    other.data_.buffer_needs_cleanup = false;
    dh_free_filter(&other.data_);
    return *this;
}

double filter::update(double in) {
    double rv = 0.0;
    if(dh_filter(&data_,in,&rv) != DH_FILTER_OK) {
        throw error("Failed to update the filter! Filter was probably moved from.");
    }
    return rv;
}


void filter::set_gain(double gain) {
    if(dh_filter_set_gain(&data_,gain) != DH_FILTER_OK) {
        throw error("Failed to set gain! Filter was probably moved from.");
    }
}

double filter::gain() const {
    double rv = 0.0;
    if(dh_filter_get_gain(&data_,&rv) != DH_FILTER_OK) {
        throw error("Failed to get gain! Filter was probably moved from.");
    }
    return rv;
}

std::vector<dh_frequency_response_t> filter::compute_frequency_response(size_t count) const {
    if(!good()) {
        throw error("Failed to compute frequency response! Filter was probably moved from.");
    }
    std::vector<dh_frequency_response_t> rv{};
    rv.reserve(count);
    for(size_t i=0;i<=count;i++) {
        dh_frequency_response_t resp{};
        resp.frequency = static_cast<double>(i)/static_cast<double>(2*count);
        auto status = dh_filter_get_gain_at(&data_, resp.frequency, &resp);
        if(status != DH_FILTER_OK) {
            throw error("Failed to get gain!");
        }
        resp.frequency *= options_.sampling_frequency;
        rv.emplace_back(resp);
    }
    return rv;
}

bool  filter::good() const noexcept {
    return data_.number_coefficients_in!=0;
}

filter::span filter::feedforward_coefficients() const noexcept {
    if(data_.number_coefficients_in>0) {
        return span(data_.coefficients_in,data_.number_coefficients_in);
    } else {
        return span(nullptr,0);
    }
}

filter::span filter::feedback_coefficients() const noexcept {
    if(data_.number_coefficients_out>0) {
        return span(data_.coefficients_out,data_.number_coefficients_out);
    } else {
        return span(nullptr,0);
    }
}

}
