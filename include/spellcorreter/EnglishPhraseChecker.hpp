#pragma once
#include <string>
#include <unordered_map>

#include "spellcorreter/IChecker.hpp"

namespace sc {

// 2~3단어 구(phrase) 문법 규칙: could of, he don't, its a, better then ...
class EnglishPhraseChecker : public IChecker {
public:
    EnglishPhraseChecker();
    std::string name() const override { return "EnglishPhraseChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;

private:
    std::unordered_map<std::string, std::pair<std::string, std::string>> phrases_;
};

}  // namespace sc
