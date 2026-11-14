#pragma once
#include <cstddef>
#include <string>
#include "spellcorreter/Utf8.hpp"

namespace sc {

// 검사기가 발견한 하나의 교정 제안. 위치는 코드포인트 오프셋.
struct Issue {
    size_t pos = 0;
    size_t len = 0;
    std::u32string repl;
    bool hasFix = true;     // false면 경고만 하고 자동 교정은 하지 않음
    std::string category;   // 철자 / 문법 / 맞춤법 / 띄어쓰기
    std::string lang;       // EN / KO
    std::string message;
    std::string source;     // 이 이슈를 만든 검사기 이름 (디버깅용)
};

inline Issue makeIssue(size_t pos, size_t len, const std::string& repl8, std::string category,
                       std::string lang, std::string message, std::string source,
                       bool hasFix = true) {
    Issue is;
    is.pos = pos;
    is.len = len;
    is.repl = text::toU32(repl8);
    is.hasFix = hasFix;
    is.category = std::move(category);
    is.lang = std::move(lang);
    is.message = std::move(message);
    is.source = std::move(source);
    return is;
}

}  // namespace sc
