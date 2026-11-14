#pragma once
#include <string>
#include <vector>

namespace sc {

struct KoreanRule {
    enum class Mode { Contains, Prefix, Whole };  // 단어 어디든 / 단어 앞부분 / 단어 전체
    std::u32string wrong;
    std::u32string right;
    std::string note;
    Mode mode = Mode::Contains;
};

// 한국어 "틀린 표기 → 올바른 표기" 규칙 저장소 (내장 + 사용자 파일).
class KoreanRuleTable {
public:
    KoreanRuleTable();

    // 사용자 규칙 파일: "틀린말=>올바른말" 또는 탭 구분, '#'은 주석. 추가된 규칙 수 반환.
    size_t loadUserRules(const std::string& path);
    void addRule(const std::string& wrong, const std::string& right, const std::string& note,
                 KoreanRule::Mode mode = KoreanRule::Mode::Contains);

    const std::vector<KoreanRule>& rules() const { return rules_; }  // 긴 규칙 우선 정렬

private:
    void sortRules();
    std::vector<KoreanRule> rules_;
};

}  // namespace sc
