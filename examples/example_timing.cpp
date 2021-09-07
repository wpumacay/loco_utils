#include <thread>
#include <tinyutils/common.hpp>
#include <tinyutils/timing.hpp>

int main() {
    tiny::utils::Logger::Init();
    tiny::utils::Clock::Init();

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

    const ssize_t num_steps = 1000;
    for (ssize_t i = 0; i < num_steps; i++) {
        tiny::utils::Clock::Tick();
        tiny::utils::Clock::Tick("lapse_1");
        std::this_thread::sleep_for(std::chrono::microseconds(11000));
        tiny::utils::Clock::Tock("lapse_1");

        tiny::utils::Clock::Tick("lapse_2");
        tiny::utils::Clock::Tick("lapse_3");
        tiny::utils::Clock::Tick("lapse_4");
        std::this_thread::sleep_for(std::chrono::microseconds(4000));
        tiny::utils::Clock::Tock("lapse_2");

        std::this_thread::sleep_for(std::chrono::microseconds(3000));
        tiny::utils::Clock::Tock("lapse_3");

        std::this_thread::sleep_for(std::chrono::microseconds(2000));
        tiny::utils::Clock::Tock("lapse_4");
        tiny::utils::Clock::Tock();

        LOG_TRACE("wall-time      : {0}", tiny::utils::Clock::GetWallTime());
        LOG_TRACE("time-step      : {0}", tiny::utils::Clock::GetTimeStep());
        LOG_TRACE("avg-time-step  : {0}", tiny::utils::Clock::GetAvgTimeStep());
        LOG_TRACE("fps            : {0}", tiny::utils::Clock::GetFps());
        LOG_TRACE("avg-fps        : {0}", tiny::utils::Clock::GetAvgFps());
        LOG_TRACE("lapse_1.step   : {0}",
                  tiny::utils::Clock::GetEvent("lapse_1").time_duration);
        LOG_TRACE("lapse_2.step   : {0}",
                  tiny::utils::Clock::GetEvent("lapse_2").time_duration);
        LOG_TRACE("lapse_3.step   : {0}",
                  tiny::utils::Clock::GetEvent("lapse_3").time_duration);
        LOG_TRACE("lapse_4.step   : {0}",
                  tiny::utils::Clock::GetEvent("lapse_4").time_duration);
    }

    tiny::utils::Logger::Release();
    tiny::utils::Clock::Release();
    return 0;
}
