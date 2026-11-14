#pragma once
#include <string>

// 문자 판별/대소문자 유틸. 상태가 없는 순수 함수 모음.
namespace sc::chr {

constexpr int kJongRieul = 8;  // 한글 받침 'ㄹ'의 종성 인덱스

inline bool isHangulSyl(char32_t c)  { return c >= 0xAC00 && c <= 0xD7A3; }
inline int  jongseong(char32_t c)    { return static_cast<int>((c - 0xAC00) % 28); }  // 0 = 받침 없음
inline bool isAsciiAlpha(char32_t c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
inline bool isLatin(char32_t c)      { return isAsciiAlpha(c) || (c >= 0xC0 && c <= 0x24F); }
inline bool isDigit(char32_t c)      { return c >= '0' && c <= '9'; }
inline bool isApos(char32_t c)       { return c == U'\'' || c == 0x2019; }

inline bool allAscii(const std::u32string& s) {
    for (char32_t c : s) if (c >= 0x80) return false;
    return true;
}

// ASCII 소문자화 + 곡선 아포스트로피(’) 정규화
inline std::string lowerAscii(const std::u32string& s) {
    std::string out;
    for (char32_t c : s) {
        if (c == 0x2019) c = '\'';
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        out += static_cast<char>(c < 0x80 ? c : '?');
    }
    return out;
}

inline bool isUpperStr(const std::u32string& s) {  // 2글자 이상 전부 대문자
    if (s.size() < 2) return false;
    for (char32_t c : s) if (!(c >= 'A' && c <= 'Z') && !isApos(c)) return false;
    return true;
}

inline std::string capitalize(std::string s) {
    if (!s.empty() && s[0] >= 'a' && s[0] <= 'z') s[0] = static_cast<char>(s[0] - 'a' + 'A');
    return s;
}

inline std::string upper(std::string s) {
    for (char& c : s) if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
    return s;
}

// 원문의 대소문자 패턴을 교정어에 적용
inline std::string matchCase(const std::u32string& orig, const std::string& repl) {
    if (orig.empty()) return repl;
    if (isUpperStr(orig)) return upper(repl);
    if (orig[0] >= 'A' && orig[0] <= 'Z') return capitalize(repl);
    return repl;
}

inline std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

}  // namespace sc::chr
