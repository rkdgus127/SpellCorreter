// 외부 프레임워크 없는 최소 테스트 러너.  빌드 후 ./spellcorreter_tests
#include <iostream>
#include <string>

#include "spellcorreter/EnglishArticleChecker.hpp"
#include "spellcorreter/EnglishDictionary.hpp"
#include "spellcorreter/SpellCorreter.hpp"
#include "spellcorreter/Utf8.hpp"

static int g_total = 0, g_fail = 0;

#define EXPECT_EQ(actual, expected)                                                        \
    do {                                                                                   \
        ++g_total;                                                                         \
        auto a_ = (actual);                                                                \
        auto e_ = (expected);                                                              \
        if (!(a_ == e_)) {                                                                 \
            ++g_fail;                                                                      \
            std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "\n   input   : " #actual \
                      << "\n   actual  : [" << a_ << "]\n   expected: [" << e_ << "]\n";   \
        }                                                                                  \
    } while (0)

static std::string fix(const sc::SpellCorreter& c, const std::string& s) {
    return sc::text::toU8(c.correct(s).corrected);
}

int main() {
    // 작은 테스트용 사전으로 영어 철자 검사를 켠다
    sc::SpellCorreter c;
    c.dictionary().setMinUsableSize(1);
    for (const char* w : {"the", "book", "is", "good", "spelling", "i", "have", "a", "an", "apple", "we",
                          "could", "won", "hello", "world", "like", "it"})
        c.dictionary().addWord(w, 100);

    // 기반 클래스
    EXPECT_EQ(sc::text::toU8(sc::text::toU32("한글 abc 😀")), std::string("한글 abc 😀"));
    EXPECT_EQ(sc::EnglishDictionary::editDistance("teh", "the", 2), 1);
    EXPECT_EQ(sc::EnglishArticleChecker::needsAn("hour"), true);
    EXPECT_EQ(sc::EnglishArticleChecker::needsAn("university"), false);
    EXPECT_EQ(sc::Document(sc::text::toU32("Hello 세계!")).tokens().size(), size_t(4));

    // 영어
    EXPECT_EQ(fix(c, "teh book"), std::string("The book"));                       // 오타표 + 문장 첫 글자
    EXPECT_EQ(fix(c, "speling is good."), std::string("Spelling is good."));      // 사전 + 편집거리
    EXPECT_EQ(fix(c, "I have a apple."), std::string("I have an apple."));        // a/an
    EXPECT_EQ(fix(c, "the the book is good."), std::string("The book is good.")); // 중복 + 문장 첫 글자
    EXPECT_EQ(fix(c, "we could of won."), std::string("We could have won."));     // 구 규칙
    EXPECT_EQ(fix(c, "i have it."), std::string("I have it."));                   // 대문자 I
    EXPECT_EQ(fix(c, "Hello world."), std::string("Hello world."));               // 변경 없음

    // 한국어
    EXPECT_EQ(fix(c, "할수있다"), std::string("할 수 있다"));
    EXPECT_EQ(fix(c, "그건 안되요"), std::string("그건 안 돼요"));
    EXPECT_EQ(fix(c, "내일 갈께"), std::string("내일 갈게"));
    EXPECT_EQ(fix(c, "책를 읽었다."), std::string("책을 읽었다."));
    EXPECT_EQ(fix(c, "금새 몇일 구지"), std::string("금세 며칠 굳이"));
    EXPECT_EQ(fix(c, "할것이다"), std::string("할 것이다"));
    EXPECT_EQ(fix(c, "안녕하세요 좋은 하루 되세요"), std::string("안녕하세요 좋은 하루 되세요"));

    // 혼합 문단
    EXPECT_EQ(fix(c, "i recieve 책를 읽을수있다."), std::string("I receive 책을 읽을 수 있다."));

    std::cout << (g_total - g_fail) << "/" << g_total << " passed\n";
    return g_fail == 0 ? 0 : 1;
}
