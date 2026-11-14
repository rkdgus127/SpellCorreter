#include "spellcorreter/KoreanSpacingChecker.hpp"

#include "spellcorreter/CharUtil.hpp"

namespace sc {

void KoreanSpacingChecker::check(const Document& doc, std::vector<Issue>& out) const {
    const auto& toks = doc.tokens();
    for (size_t i = 0; i < toks.size(); ++i) {
        if (toks[i].kind != TokenKind::Hangul) continue;
        checkNegation(doc, i, out);
        checkDependentNouns(doc, i, out);
    }
}

// '안되', '안돼' → '안 되', '안 돼'
void KoreanSpacingChecker::checkNegation(const Document& doc, size_t i, std::vector<Issue>& out) const {
    std::u32string w = doc.tokenText(i);
    if (w.size() >= 2 && w[0] == U'안' && (w[1] == U'되' || w[1] == U'돼'))
        out.push_back(makeIssue(doc.tokens()[i].pos, 2, w[1] == U'되' ? "안 되" : "안 돼", "띄어쓰기", "KO",
                                "부정 부사 '안'은 뒤 용언과 띄어 씁니다", name()));
}

// ㄹ 받침 관형형 뒤의 의존명사 '수', '것' (할수있다 → 할 수 있다, 할것이다 → 할 것이다)
void KoreanSpacingChecker::checkDependentNouns(const Document& doc, size_t i, std::vector<Issue>& out) const {
    std::u32string w = doc.tokenText(i);
    const size_t base = doc.tokens()[i].pos;
    for (size_t k = 1; k + 1 < w.size(); ++k) {
        if (!chr::isHangulSyl(w[k - 1]) || chr::jongseong(w[k - 1]) != chr::kJongRieul) continue;
        const char32_t nx = w[k + 1];
        if (w[k] == U'수') {
            if (nx == U'있' || nx == U'없')
                out.push_back(makeIssue(base + k, 1, " 수 ", "띄어쓰기", "KO", "의존명사 '수'는 앞뒤를 띄어 씁니다", name()));
            else if (nx == U'밖' || nx == U'도' || nx == U'는' || nx == U'가')
                out.push_back(makeIssue(base + k, 1, " 수", "띄어쓰기", "KO", "의존명사 '수'는 앞말과 띄어 씁니다", name()));
        } else if (w[k] == U'것') {
            if (nx == U'이' || nx == U'같' || nx == U'입' || nx == U'인')
                out.push_back(makeIssue(base + k, 1, " 것", "띄어쓰기", "KO", "의존명사 '것'은 앞말과 띄어 씁니다", name()));
        }
    }
}

}  // namespace sc
