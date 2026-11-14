#include "spellcorreter/EnglishSpellChecker.hpp"

#include "spellcorreter/CharUtil.hpp"
#include "spellcorreter/EnglishData.hpp"

namespace sc {

EnglishSpellChecker::EnglishSpellChecker(const EnglishDictionary& dict) : dict_(dict) {
    for (const auto& t : en::typos()) typos_[t.wrong] = {t.right, t.msg};
}

void EnglishSpellChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Latin) continue;
        std::u32string w = doc.tokenText(i);
        if (!chr::allAscii(w) || doc.isAdjacentToDigit(i)) continue;

        const std::string low = chr::lowerAscii(w);
        const std::string orig8 = text::toU8(w);
        std::string fix, msg;
        bool have = false, noFix = false;

        auto ty = typos_.find(low);
        if (ty != typos_.end()) {
            fix = ty->second.first;
            msg = ty->second.second.empty() ? "철자 오류: '" + orig8 + "'" : ty->second.second;
            have = true;
        } else if (dict_.isUsable() && w.size() >= 2 && !dict_.isKnown(low)) {
            const bool properNoun = (w[0] >= 'A' && w[0] <= 'Z') && !doc.isSentenceStart(i);
            bool mixedCase = false;
            for (size_t k = 1; k < w.size(); ++k) if (w[k] >= 'A' && w[k] <= 'Z') mixedCase = true;
            if (!properNoun && !mixedCase && !chr::isUpperStr(w)) {
                auto sug = dict_.suggest(low);
                have = true;
                if (sug.empty()) {
                    noFix = true;
                    msg = "사전에 없는 단어: '" + orig8 + "'";
                } else {
                    fix = sug[0];
                    msg = "철자 오류 가능성: '" + orig8 + "' (후보: ";
                    for (size_t k = 0; k < sug.size(); ++k) msg += (k ? ", " : "") + sug[k];
                    msg += ")";
                }
            }
        }
        if (!have) continue;

        if (!noFix) {
            fix = chr::matchCase(w, fix);
            if (doc.isSentenceStart(i)) fix = chr::capitalize(fix);
        }
        out.push_back(makeIssue(toks[i].pos, toks[i].len, fix, "철자", "EN", msg, name(), !noFix));
    }
}

}  // namespace sc
