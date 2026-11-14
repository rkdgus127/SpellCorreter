#pragma once
#include <iosfwd>

#include "spellcorreter/SpellCorreter.hpp"

namespace sc {

// CheckResult를 사람이 읽기 좋은 형태로 출력한다 (표시 담당, 검사 로직 없음).
class ReportPrinter {
public:
    explicit ReportPrinter(std::ostream& os) : os_(os) {}

    void setShowSource(bool v) { showSource_ = v; }  // 어느 검사기가 낸 이슈인지 표시
    void printReport(const CheckResult& r) const;    // 이슈 목록 + 교정문
    void printFixOnly(const CheckResult& r) const;   // 교정문만

private:
    std::ostream& os_;
    bool showSource_ = false;
};

}  // namespace sc
