#pragma once
#include <iostream>
#include <string>

namespace sc {

// --debug 옵션으로 켜지는 간단한 로거 (stderr 출력)
class Logger {
public:
    static bool& enabled() { static bool e = false; return e; }
    static void debug(const std::string& msg) {
        if (enabled()) std::cerr << "[debug] " << msg << "\n";
    }
};

}  // namespace sc
