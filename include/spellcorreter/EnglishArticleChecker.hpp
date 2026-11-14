#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// a / an 사용 검사 (발음 기준 휴리스틱)
class EnglishArticleChecker : public IChecker {
public:
    std::string name() const override { return "EnglishArticleChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;

    static bool needsAn(const std::string& lowerNextWord);  // 단독 테스트 가능
};

}  // namespace sc
