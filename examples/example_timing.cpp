
#include <thread>
#include <utils/timing.hpp>

// --------------------------------
// @todo(wilbert): check/validate generated JSON file for profiling session
// @todo(wilbert): potentially update profiling after watching cppcon lec.
// --------------------------------

auto main() -> int {
    utils::Logger::Init();
    utils::Clock::Init();

    auto time_stamp_chrono = std::chrono::high_resolution_clock::now();
    auto time_stamp = std::chrono::time_point_cast<std::chrono::microseconds>(
                          time_stamp_chrono)
                          .time_since_epoch()
                          .count();
    LOG_WARN("now: {0}", time_stamp);

    time_stamp_chrono = std::chrono::high_resolution_clock::now();
    time_stamp = std::chrono::time_point_cast<std::chrono::microseconds>(
                     time_stamp_chrono)
                     .time_since_epoch()
                     .count();
    LOG_WARN("now: {0}", time_stamp);

    constexpr size_t NUM_STEPS = 10;
    for (size_t i = 0; i < NUM_STEPS; i++) {
        utils::Clock::Tick();
        utils::Clock::Tick("lapse_1");
        std::this_thread::sleep_for(std::chrono::microseconds(11000));
        utils::Clock::Tock("lapse_1");

        utils::Clock::Tick("lapse_2");
        utils::Clock::Tick("lapse_3");
        utils::Clock::Tick("lapse_4");
        std::this_thread::sleep_for(std::chrono::microseconds(4000));
        utils::Clock::Tock("lapse_2");

        std::this_thread::sleep_for(std::chrono::microseconds(3000));
        utils::Clock::Tock("lapse_3");

        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        utils::Clock::Tock("lapse_4");
        utils::Clock::Tock();

        LOG_TRACE("step           : {0}", i);
        LOG_TRACE("wall-time      : {0}", utils::Clock::GetWallTime());
        LOG_TRACE("time-step      : {0}", utils::Clock::GetTimeStep());
        LOG_TRACE("avg-time-step  : {0}", utils::Clock::GetAvgTimeStep());
        LOG_TRACE("fps            : {0}", utils::Clock::GetFps());
        LOG_TRACE("avg-fps        : {0}", utils::Clock::GetAvgFps());
        LOG_TRACE("lapse_1.step   : {0}",
                  utils::Clock::GetEvent("lapse_1").time_duration);
        LOG_TRACE("lapse_2.step   : {0}",
                  utils::Clock::GetEvent("lapse_2").time_duration);
        LOG_TRACE("lapse_3.step   : {0}",
                  utils::Clock::GetEvent("lapse_3").time_duration);
        LOG_TRACE("lapse_4.step   : {0}",
                  utils::Clock::GetEvent("lapse_4").time_duration);
    }

    utils::Logger::Release();
    utils::Clock::Release();
    return 0;
}
