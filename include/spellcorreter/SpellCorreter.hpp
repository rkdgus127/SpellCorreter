#pragma once
#include <memory>
#include <string>
#include <vector>

#include "spellcorreter/Document.hpp"
#include "spellcorreter/EnglishDictionary.hpp"
#include "spellcorreter/IChecker.hpp"
#include "spellcorreter/Issue.hpp"
#include "spellcorreter/KoreanRuleTable.hpp"

namespace sc {

struct CheckResult {
    Document doc;                 // 입력 + 토큰 분석
    std::vector<Issue> issues;    // 충돌 정리 후 최종 이슈
    std::u32string corrected;     // 교정문
};

// 검사기 파이프라인을 소유하고 실행하는 최상위 클래스.
//   입력 → Document → [IChecker...] → IssueResolver → TextCorrector → CheckResult
class SpellCorreter {
public:
    SpellCorreter();
    SpellCorreter(const SpellCorreter&) = delete;             // 검사기가 멤버를 참조하므로 복사 금지
    SpellCorreter& operator=(const SpellCorreter&) = delete;

    EnglishDictionary& dictionary() { return dict_; }
    KoreanRuleTable& koreanRules() { return koRules_; }

    void addChecker(std::unique_ptr<IChecker> checker);       // 사용자 정의 검사기 추가
    std::vector<std::string> checkerNames() const;

    CheckResult correct(const std::string& utf8Text) const;

private:
    void buildDefaultPipeline();

    EnglishDictionary dict_;
    KoreanRuleTable koRules_;
    std::vector<std::unique_ptr<IChecker>> checkers_;  // 등록 순서 = 충돌 시 우선순위
};

}  // namespace sc
