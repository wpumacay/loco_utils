#include <tinyutils/logging.hpp>

int main()
{
    tinyutils::Logger::Init( tinyutils::Logger::eType::CONSOLE_LOGGER );

    LOG_CORE_TRACE( "This is a log-trace sample, foo={0}, bar={1}", 10, 100.0 );

    return 0;
}