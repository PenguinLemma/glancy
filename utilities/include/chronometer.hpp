#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace plemma::chronometer {

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

// Class to facilitate measuring duration of chunks of code.
template <typename TimeUnitsToShow>
class SingleMeasurement
{
  public:
    // Start the chronometer
    void Start() { begin_ = Clock::now(); }
    // Stop the chronometer
    void Stop() { end_ = Clock::now(); }
    // Show results: process_name took time_elapsed units_name
    void Show(std::string const& process_name, std::string const& units_name)
    {
        std::cout << process_name << " took ";
        std::cout << std::chrono::duration_cast<TimeUnitsToShow>(end_ - begin_).count();
        std::cout << " " << units_name << std::endl;
    }

  private:
    TimePoint begin_;
    TimePoint end_;
};

// Free method to ease even more measuring the duration of anything
// with an operator(). It will create and start a SingleMeasurement,
// then call the function received and then stop and show the measurement.
// Format of written output is the same than SingleMeasurement::Show :
// process_name took time_elapsed units_name
template <typename Function, typename TimeUnitsToShow>
inline void MeasureFunction(Function f,
                            std::string const& process_name,
                            std::string const& units_name)
{
    SingleMeasurement<TimeUnitsToShow> measurement;
    measurement.Start();
    f();
    measurement.Stop();
    measurement.Show(process_name, units_name);
}

}  // namespace plemma::chronometer
