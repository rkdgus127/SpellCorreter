#include "spellcorreter/EnglishRepeatedWordChecker.hpp"

#include <unordered_set>

#include "spellcorreter/CharUtil.hpp"

namespace sc {

void EnglishRepeatedWordChecker::check(const Document& doc, std::vector<Issue>& out) const {
    static const std::unordered_set<std::string> allowed = {"had", "that", "very", "so", "no", "bye", "ha", "yes"};
    const auto& toks = doc.tokens();
    for (const auto& run : doc.englishRuns()) {
        for (size_t k = 0; k + 1 < run.size(); ++k) {
            std::u32string a = doc.tokenText(run[k]), b = doc.tokenText(run[k + 1]);
            std::string la = chr::lowerAscii(a);
            if (la != chr::lowerAscii(b) || allowed.count(la)) continue;

            size_t s = toks[run[k]].pos;
            size_t e = toks[run[k + 1]].pos + toks[run[k + 1]].len;
            std::string repl = text::toU8(a);
            if (doc.isSentenceStart(run[k])) repl = chr::capitalize(repl);
            out.push_back(makeIssue(s, e - s, repl, "문법", "EN",
                                    "단어가 반복되었습니다: '" + text::toU8(a) + "'", name()));
        }
    }
}

}  // namespace sc
