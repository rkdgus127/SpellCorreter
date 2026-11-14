#include "spellcorreter/EnglishCapitalizationChecker.hpp"

#include "spellcorreter/CharUtil.hpp"

namespace sc {

void EnglishCapitalizationChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Latin) continue;
        std::u32string w = doc.tokenText(i);
        if (!chr::allAscii(w) || doc.isAdjacentToDigit(i)) continue;
        const std::string low = chr::lowerAscii(w);

        if (low == "i" && w != U"I") {
            out.push_back(makeIssue(toks[i].pos, toks[i].len, "I", "문법", "EN",
                                    "대명사 'I'는 항상 대문자로 씁니다", name()));
        } else if ((low == "i'm" || low == "i'll" || low == "i've" || low == "i'd") && w[0] == U'i') {
            out.push_back(makeIssue(toks[i].pos, toks[i].len, "I'" + low.substr(2), "문법", "EN",
                                    "대명사 'I'는 항상 대문자로 씁니다", name()));
        } else if (doc.isSentenceStart(i) && w.size() >= 2 && w[0] >= 'a' && w[0] <= 'z') {
            out.push_back(makeIssue(toks[i].pos, toks[i].len, chr::capitalize(text::toU8(w)), "문법", "EN",
                                    "문장의 첫 글자는 대문자로 씁니다", name()));
        }
    }
}

}  // namespace sc
