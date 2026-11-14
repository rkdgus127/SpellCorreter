#pragma once
#include <string>
#include <vector>

namespace sc {

struct CliOptions {
    std::string text;      // 인자로 받은 텍스트
    std::string file;      // -f 로 받은 파일 경로
    std::string enDict;    // --en-dict
    std::string koRules;   // --ko-rules
    bool haveText = false;
    bool fixOnly = false;
    bool debug = false;
    bool help = false;
};

CliOptions parseArgs(const std::vector<std::string>& args);  // 프로그램 이름 제외
void printUsage();

}  // namespace sc
