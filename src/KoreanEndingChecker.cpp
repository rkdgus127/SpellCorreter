#include "spellcorreter/KoreanEndingChecker.hpp"

#include "spellcorreter/CharUtil.hpp"

namespace sc {

void KoreanEndingChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Hangul) continue;
        std::u32string w = doc.tokenText(i);
        for (size_t k = 1; k < w.size(); ++k) {
            if (w[k] == U'께' && chr::jongseong(w[k - 1]) == chr::kJongRieul)
                out.push_back(makeIssue(toks[i].pos + k, 1, "게", "맞춤법", "KO",
                                        "'-ㄹ게'가 올바른 어미 표기입니다 ('-ㄹ께'는 틀림)", name()));
        }
    }
}

}  // namespace sc
