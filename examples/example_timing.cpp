
#include <loco/utils/timing.hpp>
#include <thread>

// --------------------------------
// @todo(wilbert): check/validate generated JSON file for profiling session
// @todo(wilbert): potentially update profiling after watching cppcon lec.
// --------------------------------

auto main() -> int {
    loco::utils::Logger::Init();
    loco::utils::Clock::Init();

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
        loco::utils::Clock::Tick();
        loco::utils::Clock::Tick("lapse_1");
        std::this_thread::sleep_for(std::chrono::microseconds(11000));
        loco::utils::Clock::Tock("lapse_1");

        loco::utils::Clock::Tick("lapse_2");
        loco::utils::Clock::Tick("lapse_3");
        loco::utils::Clock::Tick("lapse_4");
        std::this_thread::sleep_for(std::chrono::microseconds(4000));
        loco::utils::Clock::Tock("lapse_2");

        std::this_thread::sleep_for(std::chrono::microseconds(3000));
        loco::utils::Clock::Tock("lapse_3");

        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        loco::utils::Clock::Tock("lapse_4");
        loco::utils::Clock::Tock();

        LOG_TRACE("step           : {0}", i);
        LOG_TRACE("wall-time      : {0}", loco::utils::Clock::GetWallTime());
        LOG_TRACE("time-step      : {0}", loco::utils::Clock::GetTimeStep());
        LOG_TRACE("avg-time-step  : {0}", loco::utils::Clock::GetAvgTimeStep());
        LOG_TRACE("fps            : {0}", loco::utils::Clock::GetFps());
        LOG_TRACE("avg-fps        : {0}", loco::utils::Clock::GetAvgFps());
        LOG_TRACE("lapse_1.step   : {0}",
                  loco::utils::Clock::GetEvent("lapse_1").time_duration);
        LOG_TRACE("lapse_2.step   : {0}",
                  loco::utils::Clock::GetEvent("lapse_2").time_duration);
        LOG_TRACE("lapse_3.step   : {0}",
                  loco::utils::Clock::GetEvent("lapse_3").time_duration);
        LOG_TRACE("lapse_4.step   : {0}",
                  loco::utils::Clock::GetEvent("lapse_4").time_duration);
    }

    loco::utils::Logger::Release();
    loco::utils::Clock::Release();
    return 0;
}
