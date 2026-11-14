#include "spellcorreter/Document.hpp"

#include <ostream>

#include "spellcorreter/CharUtil.hpp"
#include "spellcorreter/Utf8.hpp"

namespace sc {

namespace {
TokenKind kindOf(char32_t c) {
    if (chr::isHangulSyl(c)) return TokenKind::Hangul;
    if (chr::isLatin(c)) return TokenKind::Latin;
    return TokenKind::Other;
}
}  // namespace

Document::Document(std::u32string text) : text_(std::move(text)) {
    tokenize();
    markSentenceStarts();
    buildEnglishRuns();
}

std::u32string Document::tokenText(size_t i) const {
    return text_.substr(tokens_[i].pos, tokens_[i].len);
}

void Document::tokenize() {
    size_t i = 0;
    while (i < text_.size()) {
        TokenKind k = kindOf(text_[i]);
        size_t j = i + 1;
        if (k == TokenKind::Latin) {
            while (j < text_.size()) {
                if (chr::isLatin(text_[j])) ++j;
                else if (chr::isApos(text_[j]) && j + 1 < text_.size() && chr::isLatin(text_[j + 1])) ++j;
                else break;
            }
        } else {
            while (j < text_.size() && kindOf(text_[j]) == k) ++j;
        }
        tokens_.push_back({k, i, j - i});
        i = j;
    }
}

bool Document::endsSentence(const Token& t) const {
    for (size_t k = t.pos; k < t.pos + t.len; ++k) {
        char32_t c = text_[k];
        if (c == U'\n') return true;
        if (c == U'.' || c == U'!' || c == U'?') {
            if (k + 1 == text_.size()) return true;
            char32_t n = text_[k + 1];
            if (n == U' ' || n == U'\n' || n == U'\t') return true;
        }
    }
    return false;
}

void Document::markSentenceStarts() {
    sentenceStart_.assign(tokens_.size(), false);
    bool atStart = true;
    for (size_t i = 0; i < tokens_.size(); ++i) {
        switch (tokens_[i].kind) {
            case TokenKind::Latin:  sentenceStart_[i] = atStart; atStart = false; break;
            case TokenKind::Hangul: atStart = false; break;
            case TokenKind::Other:  if (endsSentence(tokens_[i])) atStart = true; break;
        }
    }
}

bool Document::isBlankToken(size_t i) const {
    const Token& t = tokens_[i];
    if (t.kind != TokenKind::Other) return false;
    for (size_t k = t.pos; k < t.pos + t.len; ++k)
        if (text_[k] != U' ' && text_[k] != U'\t') return false;
    return true;
}

void Document::buildEnglishRuns() {
    for (size_t i = 0; i < tokens_.size(); ++i) {
        if (tokens_[i].kind != TokenKind::Latin || !chr::allAscii(tokenText(i))) continue;
        bool cont = i >= 2 && !runs_.empty() && runs_.back().back() == i - 2 && isBlankToken(i - 1);
        if (cont) runs_.back().push_back(i);
        else runs_.push_back({i});
    }
}

bool Document::isSentenceStart(size_t i) const { return sentenceStart_[i]; }

bool Document::isAdjacentToDigit(size_t i) const {
    const Token& t = tokens_[i];
    auto bad = [](char32_t c) { return chr::isDigit(c) || c == U'_'; };
    if (t.pos > 0 && bad(text_[t.pos - 1])) return true;
    size_t e = t.pos + t.len;
    return e < text_.size() && bad(text_[e]);
}

void Document::lineCol(size_t pos, size_t& line, size_t& col) const {
    line = 1;
    col = 1;
    for (size_t i = 0; i < pos && i < text_.size(); ++i) {
        if (text_[i] == U'\n') { ++line; col = 1; } else ++col;
    }
}

void Document::dumpTokens(std::ostream& os) const {
    static const char* names[] = {"Hangul", "Latin ", "Other "};
    for (size_t i = 0; i < tokens_.size(); ++i) {
        std::string s = text::toU8(tokenText(i)), esc;
        for (char c : s) esc += (c == '\n') ? std::string("\\n") : std::string(1, c);
        os << "  #" << i << " " << names[static_cast<int>(tokens_[i].kind)] << " ["
           << tokens_[i].pos << "," << tokens_[i].pos + tokens_[i].len << ") \"" << esc << "\""
           << (sentenceStart_[i] ? "  <sentence-start>" : "") << "\n";
    }
}

}  // namespace sc
