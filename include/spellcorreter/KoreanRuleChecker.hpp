#pragma once
#include "spellcorreter/IChecker.hpp"
#include "spellcorreter/KoreanRuleTable.hpp"

namespace sc {

// 규칙표 기반 맞춤법 검사 (되요→돼요, 몇일→며칠 ...)
class KoreanRuleChecker : public IChecker {
public:
    explicit KoreanRuleChecker(const KoreanRuleTable& table) : table_(table) {}
    std::string name() const override { return "KoreanRuleChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;

private:
    const KoreanRuleTable& table_;
};

}  // namespace sc
