#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// 대명사 I, 문장 첫 글자 대문자 검사
class EnglishCapitalizationChecker : public IChecker {
public:
    std::string name() const override { return "EnglishCapitalizationChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;
};

}  // namespace sc
