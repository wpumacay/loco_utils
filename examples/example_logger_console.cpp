#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>

int main()
{
    tinyutils::Logger::Init( tinyutils::Logger::eType::CONSOLE_LOGGER );

    auto vec_str = tinyutils::Split( "comp1/comp2", '/' );
    assert( vec_str.size() == 2 );

    auto buffer = std::make_unique<uint8_t[]>( 100 );
    auto address = tinyutils::PointerToHexAddress( buffer.get() );

    // Core logs
    {
        LOG_CORE_TRACE( "This is a core log-trace sample" );
        LOG_CORE_INFO( "Just an address: {0}", address );
        for ( ssize_t i = 0; i < vec_str.size(); i++ )
            LOG_CORE_WARN( "comp: {0}", vec_str[i] );
    }

    // User logs
    {
        LOG_TRACE( "This is an user log-trace sample" );
        LOG_INFO( "Just an address: {0}", address );
        for ( ssize_t i = 0; i < vec_str.size(); i++ )
            LOG_WARN( "comp: {0}", vec_str[i] );
    }

    return 0;
}