#pragma once
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sc {

// 영어 단어 사전: 로딩, 단어 조회, 편집거리 기반 후보 추천을 담당한다.
class EnglishDictionary {
public:
    EnglishDictionary();

    // 파일 로드: 한 줄에 "단어" 또는 "단어<공백/탭>빈도". 새로 추가된 단어 수를 반환.
    size_t loadFromFile(const std::string& path);
    // 후보 경로를 차례로 시도해 처음으로 성공한 파일을 로드. usedPath에 경로 저장.
    size_t loadFirstAvailable(const std::vector<std::string>& paths, std::string* usedPath = nullptr);

    bool addWord(const std::string& word, long freq = 0);  // 테스트/확장용

    size_t size() const { return words_.size(); }
    // 사전이 충분히 커야 "사전에 없는 단어" 검사를 켠다 (작은 사전의 오탐 방지).
    bool isUsable() const { return words_.size() >= minUsableSize_; }
    void setMinUsableSize(size_t n) { minUsableSize_ = n; }

    bool contains(const std::string& lowerWord) const;
    bool isKnown(const std::string& lowerWord) const;  // 축약형/소유격 포함
    std::vector<std::string> suggest(const std::string& lowerWord, size_t maxCount = 3) const;

    // 제한형 Damerau-Levenshtein 거리(OSA). maxDist 초과 시 maxDist+1.
    static int editDistance(const std::string& a, const std::string& b, int maxDist);

private:
    std::unordered_set<std::string> set_;
    std::unordered_set<std::string> contractions_;
    std::vector<std::pair<std::string, long>> words_;
    size_t minUsableSize_ = 1000;
};

}  // namespace sc
