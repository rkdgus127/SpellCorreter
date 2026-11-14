#include "spellcorreter/ReportPrinter.hpp"

#include <ostream>

namespace sc {

void ReportPrinter::printFixOnly(const CheckResult& r) const {
    std::string s = text::toU8(r.corrected);
    os_ << s;
    if (s.empty() || s.back() != '\n') os_ << '\n';
}

void ReportPrinter::printReport(const CheckResult& r) const {
    if (r.issues.empty()) {
        os_ << "  ✓ 발견된 오류가 없습니다.\n";
        return;
    }
    os_ << "  " << r.issues.size() << "개의 교정 제안\n";
    int idx = 1;
    for (const Issue& is : r.issues) {
        size_t line, col;
        r.doc.lineCol(is.pos, line, col);
        os_ << "  [" << idx++ << "] " << line << ":" << col << "  [" << is.lang << "/" << is.category << "]  \""
            << text::toU8(r.doc.text().substr(is.pos, is.len)) << "\"";
        if (is.hasFix) os_ << "  →  \"" << text::toU8(is.repl) << "\"";
        os_ << "\n       " << is.message;
        if (showSource_) os_ << "   <" << is.source << ">";
        os_ << "\n";
    }
    os_ << "\n  교정 결과:\n  " << text::toU8(r.corrected) << "\n";
}

}  // namespace sc
