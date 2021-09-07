#pragma once

#include <chrono>
#include <cstring>
#include <fstream>
#include <tinyutils/logging.hpp>
#include <unordered_map>

// Adapted from TheCherno's tutorial on profiling:
// video    : https://youtu.be/xlAH4dbMVnU
// gist     : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

constexpr const char* DEFAULT_SESSION = "session_default";

namespace tiny {
namespace utils {

/// Result object returned by profiling functions
struct ProfilerResult {
    /// Unique identifier for this profiling result
    std::string name;
    /// Starting timestamp (in microseconds)
    long long time_start;
    /// Finishing timestamp (in microseconds)
    long long time_end;
    /// Time duration (in milliseconds)
    double time_duration;
};

/// Scoped profiling timer (tracks time of a function scope)
class ProfilerTimer {
   public:
    /// Creates and initializes a scoped-timer
    ProfilerTimer(const std::string& name, const std::string& session,
                  const bool& verbose = false);

    /// Stops timer execution and releases scoped-timer resources
    ~ProfilerTimer();

   private:
    /// Stops the timer execution
    void _Stop();

   private:
    /// Unique identifier of the timer
    std::string m_Name;
    /// Session identifier (to which session is associated)
    std::string m_Session;
    /// Flag used to check if timer has stopped
    bool m_Stopped;
    /// Flag used to check if in verbose mode (prints results to stdout)
    bool m_Verbose;
    /// Time stamp of the start of the timer
    std::chrono::time_point<std::chrono::high_resolution_clock>
        m_TimePointStart;
};

/// Interface for profiling sessions, which are used to handle profiling
/// results (saving to disk, etc.)
class IProfilerSession {
   public:
    /// Available types of sessions
    enum class eType : uint8_t {
        /// Internal type of session, stores all sessions results in a
        /// buffer
        /// for internal tools to use
        INTERNAL,
        /// External-chrome type of session, saves to disk (.json) all
        /// session
        /// results in a format for the chrome-tracing tool to read
        EXTERNAL_CHROME
    };

    /// State of the session
    enum class eState : uint8_t {
        /// Idle state, session is not waiting for any result at the moment
        IDLE,
        /// Running state, session is waiting and saving|storing the
        /// provided
        /// results
        RUNNING
    };

    /// Creates a profiler session with given name
    IProfilerSession(const std::string& name)
        : m_Name(name), m_State(eState::IDLE){};

    /// Releases resources allocated by the session
    virtual ~IProfilerSession() = default;

    /// Sends this session to its running state (to keep track of profiling
    /// results)
    virtual void Begin() = 0;

    /// Writes profiling result (to disk, stores it, etc.) depending of
    /// profiler type
    virtual void Write(const ProfilerResult& result) = 0;

    /// Sends this session to its idle state (stops keeping track of
    /// profiling results)
    virtual void End() = 0;

    /// Gets the type of this session
    eType type() const { return m_Type; }

    /// Gets the current state of this session
    eState state() const { return m_State; }

    /// Gets the name of this session
    std::string name() const { return m_Name; }

   protected:
    /// Unique identifier of this session
    std::string m_Name;
    /// Type of this profiling session
    eType m_Type;
    /// Current state of the profiling session
    eState m_State;
};

/// Profiling session that stores results for later usage of internal
/// tooling
class ProfilerSessionInternal : public IProfilerSession {
   public:
    /// Creates a session that stores profiling results for usage with
    /// internal tooling
    ProfilerSessionInternal(const std::string& name);

    // Documentation inherited
    ~ProfilerSessionInternal() = default;

    // Documentation inherited
    void Begin() override;

    /// Stores the result into a container for later usage
    void Write(const ProfilerResult& result) override;

    // Documentation inherited
    void End() override;

    /// Returns the profiler-results stored so far
    std::vector<ProfilerResult> results() const { return m_Results; }

   private:
    /// Container used to store sessino results for later usage
    std::vector<ProfilerResult> m_Results;
};

/// Profiling session that saves the results to disk in the chrome-tracing
/// tool required format
class ProfilerSessionExtChrome : public IProfilerSession {
   public:
    /// Creates a session that saves its results to disk in the
    /// chrome-tracing tool format (.json)
    ProfilerSessionExtChrome(const std::string& name);

    // Documentation inherited
    ~ProfilerSessionExtChrome() = default;

    // Documentation inherited
    void Begin() override;

    /// Appends the result into a .json file for later inspection using
    /// chrome's tracing tool
    void Write(const ProfilerResult& result) override;

    // Documentation inherited
    void End() override;

   private:
    /// Writes header-part of the required chrome-tracing tool format
    void _WriteHeader();

    /// Writes footer-part of the required chrome-tracing tool format
    void _WriteFooter();

   private:
    /// File handle used to save results to disk
    std::ofstream m_FileWriter;
};

/// Profiler module(singleton) with support for multiple sessions
class Profiler {
   public:
    /// Initializes profiler module(singleton)
    static void Init(const IProfilerSession::eType& type =
                         IProfilerSession::eType::EXTERNAL_CHROME);

    /// Releases resources used by the profiler module(singleton)
    static void Release();

    /// Starts a profiling session with a given name
    static void BeginSession(const std::string& session_name);

    /// Stops an existing profiling sessino with given name
    static void EndSession(const std::string& session_name);

    /// Sends results to a profiler-session for appropriate handling
    static void WriteProfileResult(
        const ProfilerResult& result,
        const std::string& session_name = DEFAULT_SESSION);

    /// Returns all sessions currently being tracked by the profiler module
    static std::vector<IProfilerSession*> GetSessions();

    /// Releases all resources allocated by this profiler
    ~Profiler() = default;

   private:
    /// Creates a profiler and allocates all required resources
    Profiler(const IProfilerSession::eType& type) : m_ProfilerType(type) {}

    /// Start a session with a given name
    void _BeginSession(const std::string& session_name);

    /// Stops a session with a given name
    void _EndSession(const std::string& session_name);

    /// Sends results to a profiler-session for appropriate handling
    void _WriteProfileResult(const ProfilerResult& result,
                             const std::string& session_name);

    /// Returns all sessions currently being tracked
    std::vector<IProfilerSession*> _GetSessions();

   private:
    /// Handle to instance of profiler module(singleton)
    static std::unique_ptr<Profiler> s_Instance;

    /// Dictionary container for all sessions created during the module's
    /// lifetime
    std::unordered_map<std::string, std::unique_ptr<IProfilerSession>>
        m_Sessions;

    /// Type of the profiler-sessions created (either INTERNAL, or
    /// EXTERNAL_CHROME)
    IProfilerSession::eType m_ProfilerType;
};

}  // namespace utils
}  // namespace tiny

#define PROFILE_SCOPE(name) \
    tinyutils::ProfilerTimer prof_timer##__LINE__(name, DEFAULT_SESSION)
#define PROFILE_SCOPE_IN_SESSION(name, session_name) \
    tinyutils::ProfilerTimer prof_timer##__LINE__(name, session_name)

#define PROFILE_FUNCTION()                                           \
    tinyutils::ProfilerTimer prof_timer##__LINE__(__FUNCTION_NAME__, \
                                                  DEFAULT_SESSION)
#define PROFILE_FUNCTION_IN_SESSION(session_name)                    \
    tinyutils::ProfilerTimer prof_timer##__LINE__(__FUNCTION_NAME__, \
                                                  session_name)
