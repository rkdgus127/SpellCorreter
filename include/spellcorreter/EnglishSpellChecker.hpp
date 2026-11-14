#pragma once
#include <string>
#include <unordered_map>

#include "spellcorreter/EnglishDictionary.hpp"
#include "spellcorreter/IChecker.hpp"

namespace sc {

// 영어 철자 검사: (1) 흔한 오타표 (2) 사전 + 편집거리 후보 추천
class EnglishSpellChecker : public IChecker {
public:
    explicit EnglishSpellChecker(const EnglishDictionary& dict);
    std::string name() const override { return "EnglishSpellChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;

private:
    const EnglishDictionary& dict_;
    std::unordered_map<std::string, std::pair<std::string, std::string>> typos_;
};

}  // namespace sc
