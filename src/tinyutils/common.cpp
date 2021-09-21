#include <sstream>
#include <tinyutils/common.hpp>

namespace tiny {
namespace utils {

auto Split(const std::string &txt, char separator) -> std::vector<std::string> {
    std::vector<std::string> strres;

    auto pos = static_cast<int32_t>(txt.find(separator));
    if (pos == std::string::npos) {
        strres.push_back(txt);
        return strres;
    }

    int32_t initpos = 0;
    while (pos != std::string::npos) {
        strres.push_back(txt.substr(initpos, pos - initpos));
        initpos = pos + 1;

        pos = static_cast<int32_t>(txt.find(separator, initpos));
    }

    strres.push_back(txt.substr(
        initpos, std::min(pos, static_cast<int>(txt.size())) - initpos));

    return strres;
}

auto PointerToHexAddress(const void *ptr) -> std::string {
    std::stringstream ss;
    // NOLINTNEXTLINE : allow reinterpret_cast just for this
    ss << "0x" << std::hex << reinterpret_cast<std::intptr_t>(ptr);
    return ss.str();
}

}  // namespace utils
}  // namespace tiny
