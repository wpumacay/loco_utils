#include <thread>
#include <utils/logging.hpp>
#include <utils/profiling.hpp>

auto main() -> int {
    utils::Logger::Init();
    utils::Profiler::Init(utils::IProfilerSession::eType::EXTERNAL_CHROME);

    utils::Profiler::BeginSession("session_timing");
    constexpr size_t NUM_STEPS = 10;
    for (size_t i = 0; i < NUM_STEPS; i++) {
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
    utils::Profiler::EndSession("session_timing");

    utils::Logger::Release();
    utils::Profiler::Release();
    return 0;
}
