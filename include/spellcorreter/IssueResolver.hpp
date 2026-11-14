#pragma once
#include <vector>

#include "spellcorreter/Issue.hpp"

namespace sc {

// 여러 검사기가 같은 구간에 낸 이슈들의 충돌을 정리한다.
// 규칙: 시작 위치 오름차순 → 긴 범위 우선 → 교정안이 있는 것 우선 → 먼저 등록된 검사기 우선.
class IssueResolver {
public:
    static std::vector<Issue> resolve(std::vector<Issue> issues, std::vector<Issue>* dropped = nullptr);
};

}  // namespace sc
