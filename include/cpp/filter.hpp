#ifndef DH_FILTER_CPP_INCLUDED
#define DH_FILTER_CPP_INCLUDED

/** @file
 * @brief The cpp interface for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#include "dh/filter-types.h"
#include <vector>

namespace dh {

/**
 * @brief This class holds the C++ bindings to the filter library.
 * 
 */
class filter {
public:
    filter(dh_filter_parameters options);

    filter(const filter&);
    filter& operator=(const filter&);
    filter(filter&&);
    filter& operator=(filter&&);

    ~filter();

    /**
     * @brief Updates the internal state of the filter with a new input value
     * and returns the updated filtered value.
     * 
     * @param in 
     * @return double 
     */
    double update(double in);

    /**
     * @brief Returns the filtered value without changing the state of the filter.
     * 
     * @return double 
     */
    double current_value() const noexcept {
        return data_.current_value;
    }

    /** Returns the configuration of the filter. */
    const dh_filter_parameters& options() const {
        return options_;
    }
    
    /** Sets the current gain of the filter. */
    void set_gain(double gain);

    /** Gets the current gain of the filter. */
    double gain() const;

    /**
     * @brief Computes the frequency response of the filter and returns
     * a vector with count entries. 
     * 
     * The response is computed in the rang of [0,sampling_frequency/2].
     * The distance between the entries will be half of sampling_frequency/count. 
     * 
     * @param count Number values to compute
     * @return Frequency response curve. 
     */
    std::vector<dh_frequency_response_t> compute_frequency_response(size_t count) const;

    /** this class is thrown in case of errors. */
    class error {
    public:
        error(const char* str) : str_(str) {}

        const char* what() const noexcept {
            return str_;
        }

    private:
        const char* str_;
    };

    /** checks if the filter is usable. (False if object was moved from) */
    bool good() const noexcept;

    /** a simple span */
    class span {
        public:
            span(double* begin,size_t size) : begin_(begin), end_(begin+size) {}

            const double* begin() const noexcept {
                return begin_;
            }
            
            const double* end() const noexcept {
                return end_;
            }

            size_t size() const noexcept {
                return end_ - begin_;
            }

            double operator[](size_t i) const noexcept {
                return begin_[i];
            }

        private:
            const double* begin_;
            const double* end_;
    };

    /** Returns a range of the input coefficients of the filter.
     * 
     * @note May return an empty range!
     */
    span feedforward_coefficients() const noexcept;

    /** Returns a range of the output coefficients. 
     * 
     * @note May return an empty range!
     */
    span feedback_coefficients() const noexcept;

private:
    dh_filter_parameters options_{};
    dh_filter_data data_{};

    void create_internal_data();
    void deep_copy(const dh_filter_data& other);
};


}



#endif /* DH_FILTER_CPP_INCLUDED */
