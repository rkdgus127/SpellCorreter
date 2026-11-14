#include "spellcorreter/EnglishDictionary.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "spellcorreter/EnglishData.hpp"
#include "spellcorreter/Logger.hpp"

namespace sc {

EnglishDictionary::EnglishDictionary() {
    for (const char* c : en::contractions()) contractions_.insert(c);
}

bool EnglishDictionary::addWord(const std::string& word, long freq) {
    std::string w = word;
    if (w.empty()) return false;
    for (char& c : w) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
        else if (!(c >= 'a' && c <= 'z')) return false;
    }
    if (!set_.insert(w).second) return false;
    words_.push_back({w, freq});
    return true;
}

size_t EnglishDictionary::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return 0;
    std::string line;
    size_t n = 0;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string w;
        long freq = 0;
        ss >> w;
        if (!(ss >> freq)) freq = 0;
        if (addWord(w, freq)) ++n;
    }
    Logger::debug("dictionary loaded: " + path + " (" + std::to_string(n) + " words)");
    return n;
}

size_t EnglishDictionary::loadFirstAvailable(const std::vector<std::string>& paths, std::string* usedPath) {
    for (const std::string& p : paths) {
        size_t n = loadFromFile(p);
        if (n > 0) {
            if (usedPath) *usedPath = p;
            return n;
        }
    }
    return 0;
}

bool EnglishDictionary::contains(const std::string& w) const { return set_.count(w) > 0; }

bool EnglishDictionary::isKnown(const std::string& low) const {
    if (contains(low) || contractions_.count(low)) return true;
    size_t n = low.size();
    if (n > 2 && low.compare(n - 2, 2, "'s") == 0) return contains(low.substr(0, n - 2));
    if (n > 1 && low.back() == '\'') return contains(low.substr(0, n - 1));
    return false;
}

int EnglishDictionary::editDistance(const std::string& a, const std::string& b, int maxDist) {
    int n = static_cast<int>(a.size()), m = static_cast<int>(b.size());
    if (std::abs(n - m) > maxDist) return maxDist + 1;
    if (n >= 63 || m >= 63) return maxDist + 1;
    static int d[64][64];
    for (int i = 0; i <= n; ++i) d[i][0] = i;
    for (int j = 0; j <= m; ++j) d[0][j] = j;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            d[i][j] = std::min({d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost});
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1])
                d[i][j] = std::min(d[i][j], d[i - 2][j - 2] + 1);
        }
    }
    return d[n][m];
}

std::vector<std::string> EnglishDictionary::suggest(const std::string& low, size_t maxCount) const {
    struct Cand { int cost; long freq; std::string w; };
    std::vector<Cand> c;
    for (const auto& [w, f] : words_) {
        int d = editDistance(low, w, 2);
        if (d > 2) continue;
        c.push_back({d, f, w});
    }
    std::sort(c.begin(), c.end(), [&low](const Cand& a, const Cand& b) {
        if (a.cost != b.cost) return a.cost < b.cost;                 // 편집거리가 작은 것
        if (a.freq != b.freq) return a.freq > b.freq;                 // 흔한 단어
        bool fa = a.w[0] == low[0], fb = b.w[0] == low[0];
        if (fa != fb) return fa;                                      // 첫 글자가 같은 것
        return a.w < b.w;
    });
    std::vector<std::string> r;
    for (size_t i = 0; i < c.size() && i < maxCount; ++i) r.push_back(c[i].w);
    return r;
}

}  // namespace sc
