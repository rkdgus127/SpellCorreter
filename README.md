# SpellCorreter

한글/영어 맞춤법 및 문법 교정기 (C++17, 외부 라이브러리 없음)

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?logo=cplusplus)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/Python-3.8-3776AB.svg?logo=python)](https://www.python.org/)
[![License](https://img.shields.io/badge/License-GPL--3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

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

## 영어 사전
`dict/en_words.txt`(hunspell en_US를 펼친 약 12.8만 단어, 형식 `단어 점수`)가 기본 포함되어 있고,
실행 파일 옆/상위 폴더의 `dict/`를 자동으로 찾습니다. 다시 만들려면:
    python3 tools/make_en_dict.py /usr/share/hunspell/en_US dict/en_words.txt
