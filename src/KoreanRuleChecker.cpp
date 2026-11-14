#include "spellcorreter/KoreanRuleChecker.hpp"

namespace sc {

void KoreanRuleChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Hangul) continue;
        std::u32string w = doc.tokenText(i);

        for (const KoreanRule& r : table_.rules()) {
            const std::string right = text::toU8(r.right);
            switch (r.mode) {
                case KoreanRule::Mode::Whole:
                    if (w == r.wrong)
                        out.push_back(makeIssue(toks[i].pos, toks[i].len, right, "맞춤법", "KO", r.note, name()));
                    break;
                case KoreanRule::Mode::Prefix:
                    if (w.compare(0, r.wrong.size(), r.wrong) == 0)
                        out.push_back(makeIssue(toks[i].pos, r.wrong.size(), right, "맞춤법", "KO", r.note, name()));
                    break;
                case KoreanRule::Mode::Contains:
                    for (size_t p = w.find(r.wrong); p != std::u32string::npos; p = w.find(r.wrong, p + r.wrong.size()))
                        out.push_back(makeIssue(toks[i].pos + p, r.wrong.size(), right, "맞춤법", "KO", r.note, name()));
                    break;
            }
        }
    }
}

}  // namespace sc
