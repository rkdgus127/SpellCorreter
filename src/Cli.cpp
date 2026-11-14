#include "spellcorreter/Cli.hpp"

#include <iostream>

namespace sc {

CliOptions parseArgs(const std::vector<std::string>& args) {
    CliOptions o;
    const size_t argc = args.size();
    for (size_t i = 0; i < argc; ++i) {
        const std::string& a = args[i];
        if (a == "-h" || a == "--help") o.help = true;
        else if ((a == "-f" || a == "--file") && i + 1 < argc) o.file = args[++i];
        else if (a == "--fix") o.fixOnly = true;
        else if (a == "--debug") o.debug = true;
        else if (a == "--en-dict" && i + 1 < argc) o.enDict = args[++i];
        else if (a == "--ko-rules" && i + 1 < argc) o.koRules = args[++i];
        else {
            if (o.haveText) o.text += ' ';
            o.text += a;
            o.haveText = true;
        }
    }
    return o;
}

void printUsage() {
    std::cout <<
        "SpellCorreter - 한글/영어 맞춤법 및 문법 교정기\n\n"
        "사용법\n"
        "  spellcorreter                    입력창이 열리고, 문장/문단을 넣고 Enter 하면 바로 교정\n"
        "  spellcorreter \"텍스트\"           인자로 받은 텍스트 교정\n"
        "  spellcorreter -f 파일.txt         파일 교정\n"
        "  cat 파일.txt | spellcorreter      파이프 입력 (전체를 한 번에 교정)\n\n"
        "옵션\n"
        "  --fix                 교정된 텍스트만 출력\n"
        "  --debug               토큰/검사기별 이슈/충돌 제거 과정을 stderr에 출력\n"
        "  --en-dict <경로>      영어 사전 (한 줄에 '단어' 또는 '단어 빈도')\n"
        "  --ko-rules <경로>     한국어 사용자 규칙 ('틀린말=>올바른말')\n"
        "  -h, --help            도움말\n\n"
        "대화형 명령:  :debug (디버그 켜기/끄기)   quit / exit / Ctrl+D (종료)\n";
}

}  // namespace sc
