#pragma once
#include "spellcorreter/IChecker.hpp"

namespace sc {

// 조사 검사: 받침 있는 말 뒤의 '를' → '을'
class KoreanParticleChecker : public IChecker {
public:
    std::string name() const override { return "KoreanParticleChecker"; }
    void check(const Document& doc, std::vector<Issue>& out) const override;
};

}  // namespace sc
