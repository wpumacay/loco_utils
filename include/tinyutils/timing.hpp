#pragma once

#include <array>
#include <cstdint>
#include <tinyutils/logging.hpp>
#include <unordered_map>

/// Number of frames used for averaging-window
constexpr size_t NUM_FRAMES_FOR_AVG = 100;
/// Name of the default event to keep track of (wall-time)
constexpr const char* MAIN_EVENT = "walltime";

namespace tiny {
namespace utils {

struct ClockEvent {
    /// Unique identifier of the event
    std::string name;
    /// Starting time-stamp of the event
    double time_start;
    /// Finishing time-stamps of the event
    double time_stop;
    /// Total duration(in seconds) of the event
    double time_duration;

    /// Returns the string representation of the event
    std::string ToString() const;
};

class Clock {
   public:
    /// Buffer-type used for storing times used in averaging-window
    typedef std::array<float, NUM_FRAMES_FOR_AVG> BufferArray;

    /// Initialize the clock module(singleton)
    static void Init();

    /// Releases this module(singleton) and its resources
    static void Release();

    /// Starts tracking the time, until a Tock() is received
    static void Tick(const std::string& event_name = MAIN_EVENT);

    /// Stop the tracking of time (started by a Tick()) and updates internal
    /// state
    static void Tock(const std::string& event_name = MAIN_EVENT);

    /// Returns a given event by its name
    static ClockEvent GetEvent(const std::string& event_name);

    /// Returns the current time (in seconds) since the initialization of the
    /// clock module
    static float GetWallTime();

    /// Returns the time-step (delta-time in seconds) in between the last
    /// tick-tock request
    static float GetTimeStep();

    /// Returns the average time-step (average delta-time in seconds) so far
    /// (since the clock module initialization)
    static float GetAvgTimeStep();

    /// Returns the fps computed for the last tick-tock request
    static float GetFps();

    /// Returns the average fps recorded since the initialization of the clock
    /// module
    static float GetAvgFps();

    /// Returns the index of the current time-step in the times-buffer
    static size_t GetTimeIndex();

    /// Returns all elements currently being processed in the time-steps window
    static BufferArray GetTimesBuffer();

    /// Returns all elements currently being processed in the fps window
    static BufferArray GetFpsBuffer();

    /// Releases clock's module allocated resources
    ~Clock() = default;

   private:
    /// Creates a clock and allocates required resources
    Clock() = default;

    /// Starts tracking the time of a step
    void _Tick(const std::string& event);

    /// Stops the tracking of time of the current step, and updates internal
    /// state
    void _Tock(const std::string& event);

    /// Returns the time-stamp in seconds since the start of the epoch
    double _TimeStampNow();

   private:
    /// Handle to instance of clock module (singleton)
    static std::unique_ptr<Clock> s_Instance;
    /// Current wall time (in seconds)
    float m_TimeCurrent;
    /// Delta-time in between tick-tock calls (in seconds)
    float m_TimeStep;
    /// Average delta-time in between tick-tock calls (in seconds)
    float m_TimeStepAvg;
    /// Index used for average calculation and indexing in the times and fps
    /// buffers
    size_t m_TimeIndex;
    /// Buffer of time values in the averaging window
    BufferArray m_TimesBuffer;
    /// Buffer of fps-values in the averaging window
    BufferArray m_FpsBuffer;
    /// Dictionary used to store the events by name
    std::unordered_map<std::string, ClockEvent> m_ClockEvents;
};

}  // namespace utils
}  // namespace tiny
