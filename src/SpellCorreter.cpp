#include "spellcorreter/SpellCorreter.hpp"

#include <sstream>

#include "spellcorreter/EnglishArticleChecker.hpp"
#include "spellcorreter/EnglishCapitalizationChecker.hpp"
#include "spellcorreter/EnglishPhraseChecker.hpp"
#include "spellcorreter/EnglishRepeatedWordChecker.hpp"
#include "spellcorreter/EnglishSpellChecker.hpp"
#include "spellcorreter/IssueResolver.hpp"
#include "spellcorreter/KoreanEndingChecker.hpp"
#include "spellcorreter/KoreanParticleChecker.hpp"
#include "spellcorreter/KoreanRuleChecker.hpp"
#include "spellcorreter/KoreanSpacingChecker.hpp"
#include "spellcorreter/Logger.hpp"
#include "spellcorreter/TextCorrector.hpp"

namespace sc {

namespace {
std::string describe(const Document& doc, const Issue& is) {
    std::ostringstream os;
    os << "pos " << is.pos << "+" << is.len << " \"" << text::toU8(doc.text().substr(is.pos, is.len)) << "\"";
    if (is.hasFix) os << " -> \"" << text::toU8(is.repl) << "\"";
    os << "  (" << is.message << ")";
    return os.str();
}
}  // namespace

SpellCorreter::SpellCorreter() { buildDefaultPipeline(); }

void SpellCorreter::buildDefaultPipeline() {
    // 등록 순서가 곧 우선순위 (같은 구간/길이일 때 먼저 등록된 검사기가 이김)
    checkers_.push_back(std::make_unique<EnglishSpellChecker>(dict_));
    checkers_.push_back(std::make_unique<EnglishCapitalizationChecker>());
    checkers_.push_back(std::make_unique<EnglishRepeatedWordChecker>());
    checkers_.push_back(std::make_unique<EnglishArticleChecker>());
    checkers_.push_back(std::make_unique<EnglishPhraseChecker>());
    checkers_.push_back(std::make_unique<KoreanRuleChecker>(koRules_));
    checkers_.push_back(std::make_unique<KoreanEndingChecker>());
    checkers_.push_back(std::make_unique<KoreanSpacingChecker>());
    checkers_.push_back(std::make_unique<KoreanParticleChecker>());
}

void SpellCorreter::addChecker(std::unique_ptr<IChecker> checker) { checkers_.push_back(std::move(checker)); }

std::vector<std::string> SpellCorreter::checkerNames() const {
    std::vector<std::string> v;
    for (const auto& c : checkers_) v.push_back(c->name());
    return v;
}

CheckResult SpellCorreter::correct(const std::string& utf8Text) const {
    Document doc(text::toU32(utf8Text));

    if (Logger::enabled()) {
        std::ostringstream os;
        os << "tokens (" << doc.tokens().size() << "):\n";
        doc.dumpTokens(os);
        Logger::debug(os.str());
    }

    std::vector<Issue> all;
    for (const auto& checker : checkers_) {
        const size_t before = all.size();
        checker->check(doc, all);
        if (Logger::enabled()) {
            Logger::debug(checker->name() + ": " + std::to_string(all.size() - before) + " issue(s)");
            for (size_t i = before; i < all.size(); ++i) Logger::debug("    + " + describe(doc, all[i]));
        }
    }

    std::vector<Issue> dropped;
    std::vector<Issue> kept = IssueResolver::resolve(std::move(all), &dropped);
    if (Logger::enabled()) {
        for (const Issue& d : dropped)
            Logger::debug("dropped (overlap) [" + d.source + "] " + describe(doc, d));
    }

    std::u32string corrected = TextCorrector::apply(doc.text(), kept);
    return CheckResult{std::move(doc), std::move(kept), std::move(corrected)};
}

}  // namespace sc
