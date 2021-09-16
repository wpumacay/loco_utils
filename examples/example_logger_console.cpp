#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>

int main() {
    tiny::utils::Logger::Init(tiny::utils::Logger::eType::CONSOLE_LOGGER);

    auto vec_str = tiny::utils::Split("comp1/comp2", '/');
    assert(vec_str.size() == 2);

    auto buffer = std::make_unique<uint8_t[]>(100);
    auto address = tiny::utils::PointerToHexAddress(buffer.get());

    // Core logs
    {
        LOG_CORE_TRACE("This is a core log-trace sample");
        LOG_CORE_INFO("Just an address: {0}", address);
        for (size_t i = 0; i < vec_str.size(); i++)
            LOG_CORE_WARN("comp: {0}", vec_str[i]);
    }

    // User logs
    {
        LOG_TRACE("This is an user log-trace sample");
        LOG_INFO("Just an address: {0}", address);
        for (size_t i = 0; i < vec_str.size(); i++)
            LOG_WARN("comp: {0}", vec_str[i]);
    }

    return 0;
}
