#include "spellcorreter/EnglishArticleChecker.hpp"

#include <cstring>

#include "spellcorreter/CharUtil.hpp"

namespace sc {

bool EnglishArticleChecker::needsAn(const std::string& w) {
    // h가 묵음이라 모음 소리로 시작하는 단어
    static const char* silentH[] = {"hour", "honest", "honor", "heir"};
    for (const char* p : silentH) if (w.compare(0, std::strlen(p), p) == 0) return true;
    // 모음 철자지만 자음 소리(/j/, /w/)로 시작하는 단어
    static const char* consonantSound[] = {"univers", "unif", "unio", "unit", "uniq", "unic", "use", "usu",
                                           "uti", "ura", "uro", "eu", "one", "once", "ubiq", "ukr", "uten",
                                           "uter", "unan"};
    for (const char* p : consonantSound) if (w.compare(0, std::strlen(p), p) == 0) return false;
    return !w.empty() && std::string("aeiou").find(w[0]) != std::string::npos;
}

void EnglishArticleChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (const auto& run : doc.englishRuns()) {
        for (size_t k = 0; k + 1 < run.size(); ++k) {
            std::u32string w = doc.tokenText(run[k]);
            std::string low = chr::lowerAscii(w);
            if (low != "a" && low != "an") continue;

            std::u32string nw = doc.tokenText(run[k + 1]);
            if (chr::isUpperStr(nw) || !chr::allAscii(nw)) continue;  // 약어(FBI 등)는 제외
            std::string nl = chr::lowerAscii(nw);
            if (low == "an" && nl.compare(0, 6, "histor") == 0) continue;  // an historic 허용

            const bool an = needsAn(nl);
            if ((low == "a" && !an) || (low == "an" && an)) continue;
            out.push_back(makeIssue(toks[run[k]].pos, toks[run[k]].len, chr::matchCase(w, an ? "an" : "a"),
                                    "문법", "EN",
                                    "'" + text::toU8(nw) + "' 앞에는 '" + (an ? "an" : "a") + "'를 씁니다", name()));
        }
    }
}

}  // namespace sc
