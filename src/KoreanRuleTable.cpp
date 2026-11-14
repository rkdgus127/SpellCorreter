#include "spellcorreter/KoreanRuleTable.hpp"

#include <algorithm>
#include <fstream>

#include "spellcorreter/CharUtil.hpp"
#include "spellcorreter/Logger.hpp"
#include "spellcorreter/Utf8.hpp"

namespace sc {

namespace {
struct Raw { const char* wrong; const char* right; const char* note; char mode; };  // c / p / w
const Raw kBuiltin[] = {
    {"어떻해","어떡해","'어떻게 해'의 준말은 '어떡해'입니다",'c'},
    {"몇일","며칠","'며칠'이 표준어입니다",'c'},
    {"오랫만","오랜만","'오랜만'이 올바른 표기입니다",'c'},
    {"금새","금세","'금세'('금시에'의 준말)가 표준어입니다",'p'},
    {"웬지","왠지","'왠지'('왜인지'의 준말)가 올바릅니다",'p'},
    {"왠만","웬만","'웬만하다'가 표준어입니다",'c'},
    {"왠일","웬일","'웬일'이 올바른 표기입니다",'c'},
    {"설겆이","설거지","'설거지'가 표준어입니다",'c'},
    {"어의없","어이없","'어이없다'가 표준어입니다",'c'},
    {"희안","희한","'희한(稀罕)하다'가 올바릅니다",'c'},
    {"일일히","일일이","'일일이'로 적습니다",'c'},
    {"바램","바람","'바라다'의 명사형은 '바람' (색이 바래는 '바램'은 문맥 확인)",'c'},
    {"뵈요","봬요","'뵈어요'의 준말은 '봬요'입니다",'c'},
    {"안되요","안 돼요","'안'은 띄어 쓰고, '되어요'의 준말은 '돼요'입니다",'c'},
    {"안되서","안 돼서","'안'은 띄어 쓰고, '되어서'의 준말은 '돼서'입니다",'c'},
    {"되요","돼요","'되어요'의 준말은 '돼요'입니다",'c'},
    {"되서","돼서","'되어서'의 준말은 '돼서'입니다",'c'},
    {"됬","됐","'되었'의 준말은 '됐'입니다",'c'},
    {"됀","된","'된'이 올바른 활용입니다",'c'},
    {"않되","안 돼","부정 부사 '안'을 쓰고 띄어 씁니다",'c'},
    {"같애","같아","'같아'가 올바른 활용입니다",'c'},
    {"어쨋","어쨌","'어쨌든'이 올바릅니다",'c'},
    {"역활","역할","'역할'이 올바른 표기입니다",'c'},
    {"구지","굳이","'굳이'가 올바른 표기입니다",'w'},
    {"깨끗히","깨끗이","'깨끗이'로 적습니다",'c'},
    {"곰곰히","곰곰이","'곰곰이'로 적습니다",'c'},
    {"번번히","번번이","'번번이'로 적습니다",'c'},
    {"꼼꼼이","꼼꼼히","'꼼꼼히'로 적습니다",'c'},
    {"솔직이","솔직히","'솔직히'로 적습니다",'c'},
    {"나즈막","나지막","'나지막하다'가 표준어입니다",'c'},
    {"육계장","육개장","'육개장'이 표준어입니다",'c'},
    {"케잌","케이크","외래어 표기법: '케이크'",'c'},
    {"짜집기","짜깁기","'짜깁기'가 표준어입니다",'c'},
    {"눈쌀","눈살","'눈살'이 올바른 표기입니다",'c'},
    {"해깔리","헷갈리","'헷갈리다'가 표준어입니다",'c'},
    {"닥달","닦달","'닦달'이 올바른 표기입니다",'c'},
    {"뒷풀이","뒤풀이","'뒤풀이'가 표준어입니다",'c'},
    {"윗층","위층","'위층'이 표준어입니다",'c'},
    {"갯수","개수","'개수'가 표준어입니다",'c'},
    {"설레임","설렘","'설레다'의 명사형은 '설렘'입니다",'c'},
    {"사겼","사귀었","'사귀다'의 활용: '사귀었'(사귄)",'c'},
    {"할려고","하려고","'-려고'로 적습니다",'c'},
    {"볼려고","보려고","'-려고'로 적습니다",'c'},
    {"먹을려고","먹으려고","'-으려고'로 적습니다",'c'},
    {"읍니다","습니다","'-습니다'로 적습니다",'c'},
    {"있슴","있음","명사형은 '-음'입니다",'c'},
    {"없슴","없음","명사형은 '-음'입니다",'c'},
    {"했슴","했음","명사형은 '-음'입니다",'c'},
    {"안절부절하","안절부절못하","'안절부절못하다'가 표준어입니다",'c'},
    {"문뜩","문득","'문득'이 올바른 표기입니다",'c'},
    {"어짜피","어차피","'어차피'가 올바른 표기입니다",'c'},
    {"어줍잖","어쭙잖","'어쭙잖다'가 표준어입니다",'c'},
    {"낭떨어지","낭떠러지","'낭떠러지'가 표준어입니다",'c'},
    {"오뚜기","오뚝이","'오뚝이'가 표준어입니다",'c'},
    {"곱배기","곱빼기","'곱빼기'가 표준어입니다",'c'},
    {"무릎쓰","무릅쓰","'무릅쓰다'가 표준어입니다",'c'},
};
}  // namespace

KoreanRuleTable::KoreanRuleTable() {
    for (const Raw& r : kBuiltin) {
        auto m = r.mode == 'w' ? KoreanRule::Mode::Whole
               : r.mode == 'p' ? KoreanRule::Mode::Prefix
                               : KoreanRule::Mode::Contains;
        rules_.push_back({text::toU32(r.wrong), text::toU32(r.right), r.note, m});
    }
    sortRules();
}

void KoreanRuleTable::addRule(const std::string& wrong, const std::string& right,
                              const std::string& note, KoreanRule::Mode mode) {
    rules_.push_back({text::toU32(wrong), text::toU32(right), note, mode});
    sortRules();
}

size_t KoreanRuleTable::loadUserRules(const std::string& path) {
    std::ifstream in(path);
    if (!in) return 0;
    std::string line;
    size_t n = 0;
    while (std::getline(in, line)) {
        line = chr::trim(line);
        if (line.empty() || line[0] == '#') continue;
        size_t p = line.find("=>"), skip = 2;
        if (p == std::string::npos) { p = line.find('\t'); skip = 1; }
        if (p == std::string::npos) continue;
        std::string w = chr::trim(line.substr(0, p)), r = chr::trim(line.substr(p + skip));
        if (w.empty()) continue;
        rules_.push_back({text::toU32(w), text::toU32(r), "사용자 정의 규칙", KoreanRule::Mode::Contains});
        ++n;
    }
    sortRules();
    Logger::debug("korean user rules loaded: " + path + " (" + std::to_string(n) + ")");
    return n;
}

void KoreanRuleTable::sortRules() {
    std::stable_sort(rules_.begin(), rules_.end(),
                     [](const KoreanRule& a, const KoreanRule& b) { return a.wrong.size() > b.wrong.size(); });
}

}  // namespace sc
