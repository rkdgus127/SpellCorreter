#include "spellcorreter/EnglishPhraseChecker.hpp"

#include "spellcorreter/CharUtil.hpp"
#include "spellcorreter/EnglishData.hpp"

namespace sc {

EnglishPhraseChecker::EnglishPhraseChecker() {
    for (const auto& p : en::phrases()) phrases_[p.from] = {p.to, p.msg};
}

void EnglishPhraseChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (const auto& run : doc.englishRuns()) {
        for (size_t k = 0; k < run.size(); ++k) {
            for (size_t n : {3u, 2u}) {  // 긴 구 우선
                if (k + n > run.size()) continue;
                std::string key;
                for (size_t m = 0; m < n; ++m)
                    key += (m ? " " : "") + chr::lowerAscii(doc.tokenText(run[k + m]));
                auto it = phrases_.find(key);
                if (it == phrases_.end()) continue;

                size_t s = toks[run[k]].pos;
                size_t e = toks[run[k + n - 1]].pos + toks[run[k + n - 1]].len;
                std::string repl = it->second.first;
                std::u32string first = doc.tokenText(run[k]);
                if ((first[0] >= 'A' && first[0] <= 'Z') || doc.isSentenceStart(run[k]))
                    repl = chr::capitalize(repl);
                out.push_back(makeIssue(s, e - s, repl, "문법", "EN", it->second.second, name()));
                break;
            }
        }
    }
}

}  // namespace sc
