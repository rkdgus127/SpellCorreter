#include "spellcorreter/ConsoleEncoding.hpp"

#include <iostream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>
#else
#include <unistd.h>
#endif

namespace sc {

#ifdef _WIN32
namespace {
std::string wideToUtf8(const std::wstring& w) {
    if (w.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), &s[0], n, nullptr, nullptr);
    return s;
}
}  // namespace
#endif

void setupConsoleUtf8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

bool readLineUtf8(std::string& out) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {  // 실제 콘솔: 유니코드 API로 읽는다 (한글 입력 깨짐 방지)
        std::wstring w;
        wchar_t buf[512];
        DWORD n = 0;
        while (true) {
            if (!ReadConsoleW(h, buf, 512, &n, nullptr) || n == 0) {
                if (w.empty()) return false;
                break;
            }
            w.append(buf, n);
            if (w[0] == 0x1A) return false;  // Ctrl+Z = EOF
            if (w.back() == L'\n') break;
        }
        while (!w.empty() && (w.back() == L'\n' || w.back() == L'\r')) w.pop_back();
        out = wideToUtf8(w);
        return true;
    }
#endif
    return static_cast<bool>(std::getline(std::cin, out));
}

std::string executableDir(const char* argv0) {
    std::string path;
#ifdef _WIN32
    (void)argv0;
    wchar_t buf[MAX_PATH * 4];
    DWORD n = GetModuleFileNameW(nullptr, buf, static_cast<DWORD>(sizeof(buf) / sizeof(buf[0])));
    if (n > 0) path = wideToUtf8(std::wstring(buf, n));
#else
    char buf[4096];
    ssize_t n = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (n > 0) path.assign(buf, static_cast<size_t>(n));
    else if (argv0) path = argv0;
#endif
    size_t p = path.find_last_of("/\\");
    return p == std::string::npos ? std::string() : path.substr(0, p);
}

std::vector<std::string> utf8Args(int argc, char** argv) {
    std::vector<std::string> args;
#ifdef _WIN32
    (void)argv;
    int wargc = 0;
    LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    if (wargv) {
        for (int i = 1; i < wargc; ++i) args.push_back(wideToUtf8(wargv[i]));
        LocalFree(wargv);
        return args;
    }
#endif
    for (int i = 1; i < argc; ++i) args.push_back(argv[i]);
    return args;
}

}  // namespace sc
