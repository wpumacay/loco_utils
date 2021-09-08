
#include <chrono>
#include <tinyutils/timing.hpp>

namespace tiny {
namespace utils {

std::string ClockEvent::ToString() const {
    std::string str_rep;
    str_rep += "event   : " + name + "\n\r";
    str_rep += "start   : " + std::to_string(time_start) + "\n\r";
    str_rep += "stop    : " + std::to_string(time_stop) + "\n\r";
    str_rep += "duration: " + std::to_string(time_duration) + "\n\r";
    return str_rep;
}

std::unique_ptr<Clock> Clock::s_Instance = nullptr;

void Clock::Init() {
    if (!s_Instance) s_Instance = std::unique_ptr<Clock>(new Clock());

    // All these book-keeping variables are associated with the main event
    s_Instance->m_TimeCurrent = 0.0;
    s_Instance->m_TimeStep = 0.0;
    s_Instance->m_TimeStepAvg = 0.0;
    s_Instance->m_TimeIndex = 0;
    for (ssize_t i = 0; i < NUM_FRAMES_FOR_AVG; i++) {
        s_Instance->m_TimesBuffer[i] = 0.0;
        s_Instance->m_FpsBuffer[i] = 0.0;
    }
    s_Instance->m_ClockEvents.clear();
    /// Initialize main clock-event (default event that the clock keeps track
    /// of)
    s_Instance->m_ClockEvents[MAIN_EVENT] = ClockEvent();
    s_Instance->m_ClockEvents[MAIN_EVENT].name = MAIN_EVENT;
    s_Instance->m_ClockEvents[MAIN_EVENT].time_start =
        s_Instance->_TimeStampNow();
    s_Instance->m_ClockEvents[MAIN_EVENT].time_stop =
        s_Instance->_TimeStampNow();
    s_Instance->m_ClockEvents[MAIN_EVENT].time_duration = 0.0;
}

void Clock::Release() { s_Instance = nullptr; }

void Clock::Tick(const std::string& event_name) {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::Tick >>> Must initialize clock-module before using it");
    s_Instance->_Tick(event_name);
}

void Clock::Tock(const std::string& event_name) {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::Tock >>> Must initialize clock-module before using it");
    s_Instance->_Tock(event_name);
}

ClockEvent Clock::GetEvent(const std::string& event_name) {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetEvent >>> Must initialize clock-module before using it");
    if (s_Instance->m_ClockEvents.find(event_name) ==
        s_Instance->m_ClockEvents.end()) {
        LOG_CORE_WARN(
            "Clock::GetEvent >>> event named {0} wasn't found on the set of "
            "registered clock-events",
            event_name);
        return {event_name, 0.0, 0.0, 0.0};
    }
    return s_Instance->m_ClockEvents.at(event_name);
}

float Clock::GetWallTime() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetWallTime >>> Must initialize clock-module before using it");
    return s_Instance->m_TimeCurrent;
}

float Clock::GetTimeStep() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetTimeStep >>> Must initialize clock-module before using it");
    return s_Instance->m_TimeStep;
}

float Clock::GetAvgTimeStep() {
    LOG_CORE_ASSERT(s_Instance,
                    "Clock::GetAvgTimeStep >>> Must initialize clock-module "
                    "before using it");
    return s_Instance->m_TimeStepAvg;
}

float Clock::GetFps() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetFps >>> Must initialize clock-module before using it");
    return 1.0 / s_Instance->m_TimeStep;
}

float Clock::GetAvgFps() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetAvgFps >>> Must initialize clock-module before using it");
    return 1.0 / s_Instance->m_TimeStepAvg;
}

ssize_t Clock::GetTimeIndex() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetTimeIndex >>> Must initialize clock-module before using it");
    return s_Instance->m_TimeIndex;
}

Clock::BufferArray Clock::GetTimesBuffer() {
    LOG_CORE_ASSERT(s_Instance,
                    "Clock::GetTimesBuffer >>> Must initialize clock-module "
                    "before using it");
    return s_Instance->m_TimesBuffer;
}

Clock::BufferArray Clock::GetFpsBuffer() {
    LOG_CORE_ASSERT(
        s_Instance,
        "Clock::GetFpsBuffer >>> Must initialize clock-module before using it");
    return s_Instance->m_FpsBuffer;
}

void Clock::_Tick(const std::string& event_name) {
    if (m_ClockEvents.find(event_name) == m_ClockEvents.end()) {
        m_ClockEvents[event_name] = ClockEvent();
        m_ClockEvents[event_name].name = event_name;
    }
    m_ClockEvents[event_name].time_start = _TimeStampNow();
}

void Clock::_Tock(const std::string& event_name) {
    if (m_ClockEvents.find(event_name) == m_ClockEvents.end()) {
        LOG_CORE_WARN(
            "Clock::_Tock >>> tried calling _Tock() with a non-started "
            "clock-event \"{0}\"",
            event_name);
        return;
    }
    m_ClockEvents[event_name].time_stop = _TimeStampNow();
    m_ClockEvents[event_name].time_duration =
        m_ClockEvents[event_name].time_stop -
        m_ClockEvents[event_name].time_start;
    if (event_name == MAIN_EVENT) {
        m_TimeStep = m_ClockEvents[MAIN_EVENT].time_duration;
        m_TimeCurrent += m_TimeStep;
        m_TimeStepAvg =
            m_TimeStepAvg +
            (m_TimeStep - m_TimesBuffer[m_TimeIndex]) / NUM_FRAMES_FOR_AVG;
        m_TimesBuffer[m_TimeIndex] = m_TimeStep;
        m_FpsBuffer[m_TimeIndex] = 1.0 / m_TimeStep;
        m_TimeIndex = (m_TimeIndex + 1) % NUM_FRAMES_FOR_AVG;
    }
}

double Clock::_TimeStampNow() {
    auto time_stamp_chrono = std::chrono::high_resolution_clock::now();
    auto time_stamp = std::chrono::time_point_cast<std::chrono::milliseconds>(
                          time_stamp_chrono)
                          .time_since_epoch()
                          .count() *
                      0.001;
    return time_stamp;
}

}  // namespace utils
}  // namespace tiny
