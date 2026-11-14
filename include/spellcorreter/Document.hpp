#pragma once
#include <iosfwd>
#include <string>
#include <vector>

namespace sc {

enum class TokenKind { Hangul, Latin, Other };

struct Token {
    TokenKind kind;
    size_t pos;  // 코드포인트 오프셋
    size_t len;
};

// 입력 텍스트 + 토큰 + 문장 시작 정보 등 "검사기들이 공유하는 분석 결과".
// 모든 검사기는 Document를 읽기 전용으로만 사용한다.
class Document {
public:
    explicit Document(std::u32string text);

    const std::u32string& text() const { return text_; }
    const std::vector<Token>& tokens() const { return tokens_; }
    std::u32string tokenText(size_t i) const;

    bool isSentenceStart(size_t tokenIdx) const;       // 문장 첫 단어인가
    bool isAdjacentToDigit(size_t tokenIdx) const;     // "2nd", "x_1" 같은 경우 검사 제외용
    bool isBlankToken(size_t tokenIdx) const;          // 공백/탭만 있는 토큰

    // 공백으로만 이어진 영어 단어 연속열 (구/관사/중복 검사용). 값은 토큰 인덱스.
    const std::vector<std::vector<size_t>>& englishRuns() const { return runs_; }

    void lineCol(size_t pos, size_t& line, size_t& col) const;
    void dumpTokens(std::ostream& os) const;  // 디버깅용

private:
    void tokenize();
    void markSentenceStarts();
    void buildEnglishRuns();
    bool endsSentence(const Token& t) const;

    std::u32string text_;
    std::vector<Token> tokens_;
    std::vector<bool> sentenceStart_;
    std::vector<std::vector<size_t>> runs_;
};

}  // namespace sc
