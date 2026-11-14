#pragma once
#include <string>
#include <vector>

#include "spellcorreter/Issue.hpp"

namespace sc {

// 충돌이 정리된 이슈 목록을 원문에 적용해 교정문을 만든다.
class TextCorrector {
public:
    static std::u32string apply(const std::u32string& text, const std::vector<Issue>& issues);
};

}  // namespace sc
