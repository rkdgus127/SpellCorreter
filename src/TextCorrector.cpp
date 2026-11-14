#include "spellcorreter/TextCorrector.hpp"

namespace sc {

std::u32string TextCorrector::apply(const std::u32string& text, const std::vector<Issue>& issues) {
    std::u32string out;
    size_t cur = 0;
    for (const Issue& is : issues) {
        if (!is.hasFix || is.pos < cur) continue;
        out += text.substr(cur, is.pos - cur);
        out += is.repl;
        cur = is.pos + is.len;
    }
    out += text.substr(cur);
    return out;
}

}  // namespace sc
