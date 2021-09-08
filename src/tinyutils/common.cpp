#include <sstream>
#include <tinyutils/common.hpp>

namespace tiny {
namespace utils {

std::vector<std::string> Split(const std::string &txt, char separator) {
    std::vector<std::string> strres;

    int pos = txt.find(separator);
    if (pos == std::string::npos) {
        strres.push_back(txt);
        return strres;
    }

    int initpos = 0;

    while (pos != std::string::npos) {
        strres.push_back(txt.substr(initpos, pos - initpos));
        initpos = pos + 1;

        pos = txt.find(separator, initpos);
    }

    strres.push_back(
        txt.substr(initpos, std::min(pos, (int)txt.size()) - initpos));

    return strres;
}

std::string PointerToHexAddress(const void *ptr) {
    std::stringstream _ss;
    _ss << "0x" << std::hex << reinterpret_cast<std::intptr_t>(ptr);
    return _ss.str();
}

}  // namespace utils
}  // namespace tiny
