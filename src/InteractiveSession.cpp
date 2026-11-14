#include "spellcorreter/InteractiveSession.hpp"

#include <iostream>
#include <iterator>

#include "spellcorreter/CharUtil.hpp"
#include "spellcorreter/ConsoleEncoding.hpp"
#include "spellcorreter/Logger.hpp"

#ifdef _WIN32
#include <io.h>
#define SC_STDIN_IS_TTY() (_isatty(_fileno(stdin)) != 0)
#else
#include <unistd.h>
#define SC_STDIN_IS_TTY() (isatty(fileno(stdin)) != 0)
#endif

namespace sc {

void InteractiveSession::process(const std::string& text) const {
    CheckResult r = corrector_.correct(text);
    if (fixOnly_) printer_.printFixOnly(r);
    else printer_.printReport(r);
}

void InteractiveSession::run() {
    if (!SC_STDIN_IS_TTY()) {  // 파이프/리다이렉션: 전체를 한 번에
        std::string all((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
        process(all);
        return;
    }

    std::cout << "SpellCorreter - 문장이나 문단을 입력하고 Enter를 누르면 바로 교정합니다.\n"
                 "(종료: quit 또는 Ctrl+D / 디버그 토글: :debug)\n";
    std::string line;
    while (true) {
        std::cout << "\n입력> " << std::flush;
        if (!readLineUtf8(line)) break;
        std::string cmd = chr::trim(line);
        if (cmd.empty()) continue;
        if (cmd == "quit" || cmd == "exit" || cmd == ":q") break;
        if (cmd == ":debug") {
            Logger::enabled() = !Logger::enabled();
            std::cout << "  디버그 " << (Logger::enabled() ? "ON" : "OFF") << "\n";
            continue;
        }
        process(line);
    }
    std::cout << "\n";
}

}  // namespace sc
