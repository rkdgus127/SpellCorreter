#include "spellcorreter/IssueResolver.hpp"

#include <algorithm>

namespace sc {

std::vector<Issue> IssueResolver::resolve(std::vector<Issue> v, std::vector<Issue>* dropped) {
    std::stable_sort(v.begin(), v.end(), [](const Issue& a, const Issue& b) {
        if (a.pos != b.pos) return a.pos < b.pos;
        if (a.len != b.len) return a.len > b.len;
        return a.hasFix > b.hasFix;
    });
    std::vector<Issue> kept;
    size_t lastEnd = 0;
    for (auto& is : v) {
        if (!kept.empty() && is.pos < lastEnd) {
            if (dropped) dropped->push_back(std::move(is));
            continue;
        }
        lastEnd = std::max(lastEnd, is.pos + is.len);
        kept.push_back(std::move(is));
    }
    return kept;
}

}  // namespace sc
