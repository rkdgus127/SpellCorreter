#include "spellcorreter/KoreanParticleChecker.hpp"

#include "spellcorreter/CharUtil.hpp"

namespace sc {

void KoreanParticleChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Hangul) continue;
        std::u32string w = doc.tokenText(i);
        if (w.size() >= 2 && w.back() == U'를' && chr::jongseong(w[w.size() - 2]) != 0)
            out.push_back(makeIssue(toks[i].pos + w.size() - 1, 1, "을", "문법", "KO",
                                    "받침이 있는 말 뒤에는 목적격 조사 '을'을 씁니다", name()));
    }
}

}  // namespace sc
