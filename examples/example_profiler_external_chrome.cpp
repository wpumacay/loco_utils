#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>
#include <tinyutils/profiling.hpp>

#include <thread>

int main()
{
    tinyutils::Logger::Init();
    tinyutils::Profiler::Init( tinyutils::IProfilerSession::eType::EXTERNAL_CHROME );

    tinyutils::Profiler::BeginSession( "session_timing" );
    const ssize_t num_steps = 10;
    for ( ssize_t i = 0; i < num_steps; i++ )
    {
        {
            PROFILE_SCOPE_IN_SESSION( "part-1", "session_timing" );
            std::this_thread::sleep_for( std::chrono::microseconds( 11000 ) );
        }

        {
            PROFILE_SCOPE_IN_SESSION( "part-2", "session_timing" );
            std::this_thread::sleep_for( std::chrono::microseconds( 4000 ) );
        }

        {
            PROFILE_SCOPE_IN_SESSION( "part-3", "session_timing" );
            std::this_thread::sleep_for( std::chrono::microseconds( 3000 ) );
        }

        {
            PROFILE_SCOPE_IN_SESSION( "part-4", "session_timing" );
            std::this_thread::sleep_for( std::chrono::microseconds( 2000 ) );
        }
    }
    tinyutils::Profiler::EndSession( "session_timing" );

    tinyutils::Logger::Release();
    tinyutils::Profiler::Release();
    return 0;
}