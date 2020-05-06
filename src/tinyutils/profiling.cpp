
#include <tinyutils/profiling.hpp>

namespace tinyutils
{
    /**********************************************************************************************/
    /*                                 Scoped Profiling Timer                                     */
    /**********************************************************************************************/

    ProfilerTimer::ProfilerTimer( const std::string& name, const std::string& session, const bool& verbose )
    {
        m_Name = name;
        m_Session = session;
        m_Verbose = verbose;
        m_Stopped = false;
        m_TimePointStart = std::chrono::high_resolution_clock::now();
    }

    ProfilerTimer::~ProfilerTimer()
    {
        if ( !m_Stopped )
            _Stop();
    }

    void ProfilerTimer::_Stop()
    {
        auto time_point_end = std::chrono::high_resolution_clock::now();

        long long time_start = std::chrono::time_point_cast<std::chrono::microseconds>( m_TimePointStart ).time_since_epoch().count();
        long long time_end = std::chrono::time_point_cast<std::chrono::microseconds>( time_point_end ).time_since_epoch().count();
        double time_duration = ( time_end - time_start ) * 0.001;

        Profiler::WriteProfileResult( { m_Name, time_start, time_end, time_duration }, m_Session );
        m_Stopped = true;
    }

    /**********************************************************************************************/
    /*                                 Internal profiling session                                 */
    /**********************************************************************************************/

    ProfilerSessionInternal::ProfilerSessionInternal( const std::string& name )
        : IProfilerSession( name )
    {
        m_Type = IProfilerSession::eType::INTERNAL;
    }

    void ProfilerSessionInternal::Begin()
    {
        m_Results.clear();
        m_State = IProfilerSession::eState::RUNNING;
    }

    void ProfilerSessionInternal::Write( const ProfilerResult& result )
    {
        m_Results.push_back( result );
    }

    void ProfilerSessionInternal::End()
    {
        m_State = IProfilerSession::eState::IDLE;
    }

    /**********************************************************************************************/
    /*                              Chrome-tracing profiling session                              */
    /**********************************************************************************************/

    ProfilerSessionExtChrome::ProfilerSessionExtChrome( const std::string& name )
        : IProfilerSession( name )
    {
        m_Type = IProfilerSession::eType::EXTERNAL_CHROME;
    }

    void ProfilerSessionExtChrome::Begin()
    {
        const std::string filename = std::string( m_Name ) + ".json";
        m_FileWriter.open( filename, std::ofstream::out );
        if ( !m_FileWriter.is_open() )
        {
            LOG_CORE_WARN( "ProfilerSessionExtChrome::Begin >>> couldn't open session file {0}", filename );
            return;
        }

        _WriteHeader();
        m_State = IProfilerSession::eState::RUNNING;
    }

    void ProfilerSessionExtChrome::Write( const ProfilerResult& result )
    {
        if ( m_State != IProfilerSession::eState::RUNNING )
            return;

        std::stringstream json;
        std::string name = result.name;
        std::replace( name.begin(), name.end(), '"', '\'' );

        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << ( result.time_end - result.time_start ) << ",";
        json << "\"name\":\"" << name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":0,";
        json << "\"ts\":" << result.time_start;
        json << "}";

        m_FileWriter << json.str();
        m_FileWriter.flush();
    }

    void ProfilerSessionExtChrome::End()
    {
        if ( m_State != IProfilerSession::eState::RUNNING )
            return;

        _WriteFooter();
        m_FileWriter.close();
        m_State = IProfilerSession::eState::RUNNING;
    }

    void ProfilerSessionExtChrome::_WriteHeader()
    {
        m_FileWriter << "{\"otherData\": {},\"traceEvents\":[{}";
        m_FileWriter.flush();
    }

    void ProfilerSessionExtChrome::_WriteFooter()
    {
        m_FileWriter << "]}";
        m_FileWriter.flush();
    }

    /**********************************************************************************************/
    /*                                    Profiler module                                         */
    /**********************************************************************************************/

    std::unique_ptr<Profiler> Profiler::s_Instance = nullptr;

    void Profiler::Init( const IProfilerSession::eType& type )
    {
        if ( !s_Instance )
            s_Instance = std::unique_ptr<Profiler>( new Profiler( type ) );
        Profiler::BeginSession( DEFAULT_SESSION );
    }

    void Profiler::Release()
    {
        Profiler::EndSession( DEFAULT_SESSION );
        s_Instance = nullptr;
    }

    void Profiler::BeginSession( const std::string& session_name )
    {
        LOG_CORE_ASSERT( s_Instance, "Profiler::BeginSession >>> Profiler module must be initialized before using it" );
        s_Instance->_BeginSession( session_name );
    }

    void Profiler::EndSession( const std::string& session_name )
    {
        LOG_CORE_ASSERT( s_Instance, "Profiler::EndSession >>> Profiler module must be initialized before using it" );
        s_Instance->_EndSession( session_name );
    }

    void Profiler::WriteProfileResult( const ProfilerResult& result, const std::string& session_name )
    {
        LOG_CORE_ASSERT( s_Instance, "Profiler::WriteProfileResult >>> Profiler module must be initialized before using it" );
        s_Instance->_WriteProfileResult( result, session_name );
    }

    std::vector< IProfilerSession* > Profiler::GetSessions()
    {
        LOG_CORE_ASSERT( s_Instance, "Profiler::GetSessions >>> Profiler module must be initialized before using it" );
        return s_Instance->_GetSessions();
    }

    void Profiler::_BeginSession( const std::string& session_name )
    {
        if ( m_Sessions.find( session_name ) == m_Sessions.end() )
        {
            if ( m_ProfilerType == IProfilerSession::eType::INTERNAL )
                m_Sessions[session_name] = std::make_unique<ProfilerSessionInternal>( session_name );
            else
                m_Sessions[session_name] = std::make_unique<ProfilerSessionExtChrome>( session_name );
        }
        m_Sessions[session_name]->Begin();
    }

    void Profiler::_EndSession( const std::string& session_name )
    {
        if ( m_Sessions.find( session_name ) == m_Sessions.end() )
            LOG_CORE_WARN( "Profiler::_EndSession() >>> session with name {0} not found", session_name );
        else
            m_Sessions[session_name]->End();
    }

    void Profiler::_WriteProfileResult( const ProfilerResult& result, const std::string& session_name )
    {
        if ( m_Sessions.find( session_name ) == m_Sessions.end() )
            LOG_CORE_WARN( "Profiler::_WriteProfileResult() >>> session with name {0} not found", session_name );
        else
            m_Sessions[session_name]->Write( result );
    }

    std::vector< IProfilerSession* > Profiler::_GetSessions()
    {
        std::vector< IProfilerSession* > vec_sessions;
        for ( auto& kvpair : m_Sessions )
            vec_sessions.push_back( kvpair.second.get() );
        return vec_sessions;
    }
}