#pragma once
#include "spellcorreter/ReportPrinter.hpp"
#include "spellcorreter/SpellCorreter.hpp"

namespace sc {

// 입력창(REPL) 담당. 터미널에서 직접 실행하면 한 줄 입력할 때마다 즉시 교정하고,
// 파이프로 들어오면 전체 입력을 한 번에 교정한다.
class InteractiveSession {
public:
    InteractiveSession(const SpellCorreter& corrector, const ReportPrinter& printer, bool fixOnly)
        : corrector_(corrector), printer_(printer), fixOnly_(fixOnly) {}

    void run();

private:
    void process(const std::string& text) const;

    const SpellCorreter& corrector_;
    const ReportPrinter& printer_;
    bool fixOnly_;
};

}  // namespace sc
