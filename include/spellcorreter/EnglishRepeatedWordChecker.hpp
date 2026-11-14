#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// "the the" 같은 연속 중복 단어 검사
class EnglishRepeatedWordChecker : public IChecker {
public:
    std::string name() const override { return "EnglishRepeatedWordChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;
};

}  // namespace sc
