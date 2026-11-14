#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// 어미 표기 검사: -ㄹ께 → -ㄹ게 (할께, 갈께요 ...)
class KoreanEndingChecker : public IChecker {
public:
    std::string name() const override { return "KoreanEndingChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;
};

}  // namespace sc
