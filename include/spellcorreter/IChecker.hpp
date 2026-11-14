#pragma once
#include <string>
#include <vector>

#include "spellcorreter/Document.hpp"
#include "spellcorreter/Issue.hpp"

namespace sc {

// 모든 검사기의 공통 인터페이스. 새 규칙을 추가하려면 이 클래스를 상속해
// SpellCorreter 파이프라인에 등록하면 된다.
class IChecker {
public:
    virtual ~IChecker() = default;
    virtual std::string name() const = 0;
    virtual void check(const Document& doc, std::vector<Issue>& out) const = 0;
};

}  // namespace sc
