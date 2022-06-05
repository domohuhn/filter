#ifndef DH_FILTER_CPP_INCLUDED
#define DH_FILTER_CPP_INCLUDED

/** @file
 * @brief The cpp interface for the filter library. 
 * 
 * This source code is licensed under the MIT license. See file "LICENSE" at the root of the repository.
 */

#include "dh/filter-types.h"
#include <vector>

/**
 * @defgroup cpp-API API for C++
 * 
 * @brief The API for the C++ Programming language.
 * 
 * The C++ API provides a convienient interface that is easire to use than the C-API. 
 * To create a filter, you first create a dh_filter_parameters structure and initialize
 * its data members.
 * The filter is initialized via the constructor. If the object is returned and no exception is
 * throw, you are good to go.
 * 
 * Here is an example for the basic usage of the API:
 * ```C
 * dh::filter::parameters_t parameters;
 * // set values in parameters to create your desired filter
 * auto filter = dh::filter(parameters);
 * // the filter is ready to be used
 * while (do_stuff) {
 *     auto output = filter.update(get_inputs());
 *     // use output
 * }
 * ```
 */

namespace dh {

/**
 * @brief This class holds the C++ bindings to the filter library.
 * 
 * Simply construct it with given options and you are ready to use your filter.
 * If an error occurs, an exception will be thrown. This should either happen if
 * the given parameters are invalid, you are out of memory or if the filter
 * structure was moved from.
 * 
 * @throws dh::filter::error in case something goes wrong.
 * @ingroup cpp-API
 */
class filter {
public:
    /** Typedef to the parameter structure */
    using parameters_t = dh_filter_parameters;

    /** @brief Constructs a filter with the given options.
     * @throws dh::filter::error in case something goes wrong.
     * @see dh_filter_parameters
     */
    filter(parameters_t options);

    /** @brief Deep copy. */
    filter(const filter&);

    /** @brief Deep copy. */
    filter& operator=(const filter&);

    /** @brief Move constructor. 
     * @warning The other filter object becomes invalid and can no longer be used. */
    filter(filter&&);

    /** @brief Move assignment. 
     * @warning The other filter object becomes invalid and can no longer be used. */
    filter& operator=(filter&&);

    ~filter();

    /**
     * @brief Updates the internal state of the filter with a new input value
     * and returns the updated filtered value.
     * 
     * @param[in] in The next input value.
     * @return The current output value of the filter.
     */
    double update(double in);

    /**
     * @brief Returns the filtered value without changing the state of the filter.
     * 
     * @return The last output of update().
     */
    double current_value() const noexcept {
        return data_.current_value;
    }

    /** Returns the configuration of the filter. */
    const parameters_t& options() const {
        return options_;
    }
    
    /** Sets the current gain of the filter.
     * @param[in] gain The desired gain.
     **/
    void set_gain(double gain);

    /** Gets the current gain of the filter. */
    double gain() const;

    /**
     * @brief Computes the frequency response of the filter and returns
     * a vector with count entries. 
     * 
     * The response is computed in the range of [0,sampling_frequency/2].
     * The distance between the entries will be half of sampling_frequency/count. 
     * 
     * @param[in] count Number values to compute
     * @return Frequency response curve. 
     */
    std::vector<dh_frequency_response_t> compute_frequency_response(size_t count) const;

    /** A simple structure that is used to generate the impulse and step response. */
    struct graph_point {
        /** Position on the x-axis.*/
        double x;
        /** Input to the filter.*/
        double input;
        /** Output of the filter.*/
        double output;
    };

    /**
     * @brief Computes the step response of the filter. 
     * 
     * The response is computed in the range of [-10,2*number of feedforward coefficients].
     */
    std::vector<graph_point> compute_step_response() const;

    /**
     * @brief Computes the impulse response of the filter. 
     * 
     * The response is computed in the range of [-10,2*number of feedforward coefficients].
     */
    std::vector<graph_point> compute_impulse_response() const;

    /** This class is thrown in case of errors. */
    class error {
    public:
        /** Constructor that sets the error message. */
        error(const char* str) : str_(str) {}

        /** Returns a string describing what went wrong. */
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
            /** Constructor that sets the range. */
            span(double* begin,size_t size) : begin_(begin), end_(begin+size) {}

            /** Start of the range. */
            const double* begin() const noexcept {
                return begin_;
            }
            
            /** End of the range. */
            const double* end() const noexcept {
                return end_;
            }

            /** Number of elements in the range. */
            size_t size() const noexcept {
                return end_ - begin_;
            }

            /** Random access operator. */
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
