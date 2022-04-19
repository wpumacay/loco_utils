#include <loco/utils/logging.hpp>

auto main() -> int {
    loco::utils::Logger::Init(loco::utils::Logger::eType::CONSOLE_LOGGER);

    auto vec_str = loco::utils::Split("comp1/comp2", '/');
    assert(vec_str.size() == 2);

    constexpr size_t BUFFER_SIZE = 100;
    auto buffer = std::array<uint8_t, BUFFER_SIZE>();
    auto address = loco::utils::PointerToHexAddress(buffer.data());

    // Core logs
    {
        LOG_CORE_TRACE("This is a core log-trace sample");
        LOG_CORE_INFO("Just an address: {0}", address);
        for (const auto& str : vec_str) {
            LOG_CORE_WARN("comp: {0}", str);
        }
    }

    // User logs
    {
        LOG_TRACE("This is an user log-trace sample");
        LOG_INFO("Just an address: {0}", address);
        for (const auto& str : vec_str) {
            LOG_WARN("comp: {0}", str);
        }
    }

    return 0;
}
