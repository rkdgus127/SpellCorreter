# SpellCorreter

한글/영어 맞춤법 및 문법 교정기 (C++17, 외부 라이브러리 없음)

## 빌드
    cmake -S . -B build && cmake --build build
    ./build/spellcorreter_tests        # 테스트
    ./build/spellcorreter              # 입력창이 열리고, 문장/문단 입력 후 Enter 하면 자동 교정

CMake 없이:  g++ -std=c++17 -Iinclude src/*.cpp app/main.cpp -o spellcorreter

## 구조 (데이터 흐름)
    입력 → Document(토큰/문장시작) → [IChecker ...] → IssueResolver → TextCorrector → ReportPrinter

| 계층 | 클래스 |
|---|---|
| 기반 | Utf8, CharUtil, Logger, Issue, Document |
| 인터페이스 | IChecker |
| 영어 | EnglishDictionary, EnglishSpellChecker, EnglishCapitalizationChecker, EnglishRepeatedWordChecker, EnglishArticleChecker, EnglishPhraseChecker |
| 한국어 | KoreanRuleTable, KoreanRuleChecker, KoreanEndingChecker, KoreanSpacingChecker, KoreanParticleChecker |
| 후처리 | IssueResolver, TextCorrector, ReportPrinter |
| 조립/진입 | SpellCorreter, Cli, InteractiveSession, app/main.cpp |

## 디버깅
    ./spellcorreter --debug "I recieve a apple"
토큰 분석, 검사기별 이슈, 충돌로 버려진 이슈, 이슈를 낸 검사기 이름을 보여 줍니다.
새 규칙은 IChecker를 상속해 `SpellCorreter::addChecker()`로 추가하세요.

## 영어 사전
`dict/en_words.txt`(hunspell en_US를 펼친 약 12.8만 단어, 형식 `단어 점수`)가 기본 포함되어 있고,
실행 파일 옆/상위 폴더의 `dict/`를 자동으로 찾습니다. 다시 만들려면:
    python3 tools/make_en_dict.py /usr/share/hunspell/en_US dict/en_words.txt
