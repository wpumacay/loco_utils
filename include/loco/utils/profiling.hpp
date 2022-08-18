#pragma once

// clang-format off
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

#include <loco/utils/logging.hpp>
#include "loco/utils/common.hpp"
// clang-format on

// Adapted from TheCherno's tutorial on profiling:
// video    : https://youtu.be/xlAH4dbMVnU
// gist     : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

constexpr const char* DEFAULT_SESSION = "session_default";

namespace loco {
namespace utils {

/// Result object returned by profiling functions
struct ProfilerResult {
    /// Unique identifier for this profiling result
    std::string name = "result";
    /// Starting timestamp (in microseconds)
    int64_t time_start = 0;
    /// Finishing timestamp (in microseconds)
    int64_t time_end = 0;
    /// Time duration (in milliseconds)
    double time_duration = 0.0;
};

/// Scoped profiling timer (tracks time of a function scope)
class ProfilerTimer {
    // cppcheck-suppress unknownMacro
    LOCO_DEFINE_SMART_POINTERS(ProfilerTimer)

    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(ProfilerTimer);

 public:
    /// Creates and initializes a scoped-timer
    ProfilerTimer(std::string name, std::string session);

    /// Stops timer execution and releases scoped-timer resources
    ~ProfilerTimer();

 private:
    /// Stops the timer execution
    void _Stop();

 private:
    /// Unique identifier of the timer
    std::string m_Name;
    /// Session identifier (to which session is associated)
    std::string m_Session = DEFAULT_SESSION;
    /// Flag used to check if timer has stopped
    bool m_Stopped = false;
    /// Time stamp of the start of the timer
    std::chrono::time_point<std::chrono::high_resolution_clock>
        m_TimePointStart;
};

/// Interface for profiling sessions, which are used to handle profiling
/// results (saving to disk, etc.)
class IProfilerSession {
    // cppcheck-suppress unknownMacro
    LOCO_DEFINE_SMART_POINTERS(IProfilerSession)

    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(IProfilerSession);

 public:
    /// Available types of sessions
    enum class eType : uint8_t {
        /// Internal type of session, stores all sessions results in a
        /// buffer for internal tools to use
        INTERNAL,
        /// External-chrome type of session, saves to disk (.json) all
        /// session results in a format for the chrome-tracing tool to read
        EXTERNAL_CHROME
    };

    /// State of the session
    enum class eState : uint8_t {
        /// Idle state, session is not waiting for any result at the moment
        IDLE,
        /// Running state, session is waiting and saving|storing for results
        RUNNING
    };

    /// Creates a profiler session with given name
    explicit IProfilerSession(std::string name) : m_Name(std::move(name)) {}

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
    auto type() const -> eType { return m_Type; }

    /// Gets the current state of this session
    auto state() const -> eState { return m_State; }

    /// Gets the name of this session
    auto name() const -> std::string { return m_Name; }

 protected:
    /// Unique identifier of this session
    std::string m_Name = DEFAULT_SESSION;  // NOLINT @todo(wilbert): check later
    /// Type of this profiling session
    eType m_Type = eType::INTERNAL;  // NOLINT @todo(wilbert): check later
    /// Current state of the profiling session
    eState m_State = eState::IDLE;  // NOLINT @todo(wilbert): check later
};

/// Profiling session that stores results for later usage of internal
/// tooling
class ProfilerSessionInternal : public IProfilerSession {
    // cppcheck-suppress unknownMacro
    LOCO_DEFINE_SMART_POINTERS(ProfilerSessionInternal)

    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(ProfilerSessionInternal);

 public:
    /// Creates a session that stores profiling results for usage with
    /// internal tooling
    explicit ProfilerSessionInternal(const std::string& name);

    // Documentation inherited
    ~ProfilerSessionInternal() override = default;

    // Documentation inherited
    void Begin() override;

    /// Stores the result into a container for later usage
    void Write(const ProfilerResult& result) override;

    // Documentation inherited
    void End() override;

    /// Returns the profiler-results stored so far
    auto results() const -> std::vector<ProfilerResult> { return m_Results; }

 private:
    /// Container used to store sessino results for later usage
    std::vector<ProfilerResult> m_Results;
};

/// Profiling session that saves the results to disk in the chrome-tracing
/// tool required format
class ProfilerSessionExtChrome : public IProfilerSession {
    // cppcheck-suppress unknownMacro
    LOCO_DEFINE_SMART_POINTERS(ProfilerSessionExtChrome)

    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(ProfilerSessionExtChrome);

 public:
    /// Creates a session that saves its results to disk in the
    /// chrome-tracing tool format (.json)
    explicit ProfilerSessionExtChrome(const std::string& name);

    // Documentation inherited
    ~ProfilerSessionExtChrome() override = default;

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
    LOCO_DEFINE_SMART_POINTERS(ProfilerSessionExtChrome)

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
    static auto GetSessions() -> std::vector<IProfilerSession*>;

 private:
    /// Creates a profiler and allocates all required resources
    explicit Profiler(const IProfilerSession::eType& type)
        : m_ProfilerType(type) {}

    /// Start a session with a given name
    void _BeginSession(const std::string& session_name);

    /// Stops a session with a given name
    void _EndSession(const std::string& session_name);

    /// Sends results to a profiler-session for appropriate handling
    void _WriteProfileResult(const ProfilerResult& result,
                             const std::string& session_name);

    /// Returns all sessions currently being tracked
    auto _GetSessions() -> std::vector<IProfilerSession*>;

 private:
    /// Handle to instance of profiler module(singleton)
    // NOLINTNEXTLINE @todo(wilbert): replace singleton pattern?
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
}  // namespace loco

// NOLINTNEXTLINE
#define PROFILE_SCOPE(name) \
    loco::utils::ProfilerTimer prof_timer##__LINE__(name, DEFAULT_SESSION)
// NOLINTNEXTLINE : @todo(wilbert) check usage of constexpr template function
#define PROFILE_SCOPE_IN_SESSION(name, session_name) \
    loco::utils::ProfilerTimer prof_timer##__LINE__(name, session_name)
// NOLINTNEXTLINE : @todo(wilbert) check usage of constexpr template function
#define PROFILE_FUNCTION()                                             \
    loco::utils::ProfilerTimer prof_timer##__LINE__(__FUNCTION_NAME__, \
                                                    DEFAULT_SESSION)
// NOLINTNEXTLINE : @todo(wilbert) check usage of constexpr template function
#define PROFILE_FUNCTION_IN_SESSION(session_name)                      \
    loco::utils::ProfilerTimer prof_timer##__LINE__(__FUNCTION_NAME__, \
                                                    session_name)
