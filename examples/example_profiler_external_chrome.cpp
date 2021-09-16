#include <thread>
#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>
#include <tinyutils/profiling.hpp>

int main() {
    tiny::utils::Logger::Init();
    tiny::utils::Profiler::Init(
        tiny::utils::IProfilerSession::eType::EXTERNAL_CHROME);

    tiny::utils::Profiler::BeginSession("session_timing");
    const size_t num_steps = 10;
    for (size_t i = 0; i < num_steps; i++) {
        {
            PROFILE_SCOPE_IN_SESSION("part-1", "session_timing");
            std::this_thread::sleep_for(std::chrono::microseconds(11000));
        }

        {
            PROFILE_SCOPE_IN_SESSION("part-2", "session_timing");
            std::this_thread::sleep_for(std::chrono::microseconds(4000));
        }

        {
            PROFILE_SCOPE_IN_SESSION("part-3", "session_timing");
            std::this_thread::sleep_for(std::chrono::microseconds(3000));
        }

        {
            PROFILE_SCOPE_IN_SESSION("part-4", "session_timing");
            std::this_thread::sleep_for(std::chrono::microseconds(2000));
        }
    }
    tiny::utils::Profiler::EndSession("session_timing");

    tiny::utils::Logger::Release();
    tiny::utils::Profiler::Release();
    return 0;
}
