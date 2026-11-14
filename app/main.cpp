#include <fstream>
#include <iostream>
#include <sstream>

#include "spellcorreter/Cli.hpp"
#include "spellcorreter/ConsoleEncoding.hpp"
#include "spellcorreter/InteractiveSession.hpp"
#include "spellcorreter/Logger.hpp"
#include "spellcorreter/ReportPrinter.hpp"
#include "spellcorreter/SpellCorreter.hpp"

using namespace sc;

// 사전/규칙 파일 로딩 (실패해도 내장 규칙으로 동작)
static void loadResources(SpellCorreter& corrector, const CliOptions& opt, const std::string& exeDir) {
    std::vector<std::string> candidates;
    if (!opt.enDict.empty()) candidates.push_back(opt.enDict);
    if (!exeDir.empty()) {  // 실행 파일 옆, 그리고 빌드 폴더 기준 상위 폴더들 (프로젝트 루트의 dict/)
        for (const char* rel : {"/dict/en_words.txt", "/en_words.txt", "/../dict/en_words.txt",
                                "/../../dict/en_words.txt", "/../../../dict/en_words.txt"})
            candidates.push_back(exeDir + rel);
    }
    for (const char* p : {"dict/en_words.txt", "en_words.txt", "/usr/share/dict/words",
                          "/usr/share/dict/american-english"})
        candidates.push_back(p);

    std::string used;
    size_t n = corrector.dictionary().loadFirstAvailable(candidates, &used);
    if (n > 0) std::cerr << "[정보] 영어 사전 로드: " << used << " (" << n << "단어)\n";
    if (!corrector.dictionary().isUsable())
        std::cerr << "[안내] 영어 사전이 없어 내장 오타표/문법 규칙만 사용합니다. "
                     "--en-dict 로 사전 파일을 지정하면 철자 검사가 확장됩니다.\n";

    if (!opt.koRules.empty()) {
        size_t k = corrector.koreanRules().loadUserRules(opt.koRules);
        std::cerr << "[정보] 한국어 사용자 규칙 " << k << "개 로드\n";
    }
}

int main(int argc, char** argv) {
    setupConsoleUtf8();  // Windows 콘솔 한글 깨짐 방지
    CliOptions opt = parseArgs(utf8Args(argc, argv));
    if (opt.help) { printUsage(); return 0; }
    Logger::enabled() = opt.debug;

    SpellCorreter corrector;
    loadResources(corrector, opt, executableDir(argv[0]));

    ReportPrinter printer(std::cout);
    printer.setShowSource(opt.debug);

    auto processText = [&](const std::string& text) {
        CheckResult r = corrector.correct(text);
        if (opt.fixOnly) printer.printFixOnly(r);
        else printer.printReport(r);
    };

    if (!opt.file.empty()) {
        std::ifstream in(opt.file, std::ios::binary);
        if (!in) { std::cerr << "파일을 열 수 없습니다: " << opt.file << "\n"; return 1; }
        std::stringstream ss;
        ss << in.rdbuf();
        processText(ss.str());
        return 0;
    }
    if (opt.haveText) { processText(opt.text); return 0; }

    InteractiveSession(corrector, printer, opt.fixOnly).run();
    return 0;
}
