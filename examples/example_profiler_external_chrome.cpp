#include <loco/utils/common.hpp>
#include <loco/utils/logging.hpp>
#include <loco/utils/profiling.hpp>
#include <thread>

auto main() -> int {
    loco::utils::Logger::Init();
    loco::utils::Profiler::Init(
        loco::utils::IProfilerSession::eType::EXTERNAL_CHROME);

    loco::utils::Profiler::BeginSession("session_timing");
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
    loco::utils::Profiler::EndSession("session_timing");

    loco::utils::Logger::Release();
    loco::utils::Profiler::Release();
    return 0;
}
