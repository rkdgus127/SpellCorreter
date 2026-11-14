#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// 띄어쓰기 검사: 부정 부사 '안', 의존명사 '수'/'것'
class KoreanSpacingChecker : public IChecker {
public:
    std::string name() const override { return "KoreanSpacingChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;

private:
    void checkNegation(const Document& doc, size_t tokIdx, std::vector<Issue>& out) const;
    void checkDependentNouns(const Document& doc, size_t tokIdx, std::vector<Issue>& out) const;
};

}  // namespace sc
