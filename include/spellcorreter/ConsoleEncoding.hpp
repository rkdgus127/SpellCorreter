#pragma once
#include <string>
#include <vector>

namespace sc {

// 플랫폼별 콘솔 인코딩 문제를 한곳에서 처리한다.
//  - Windows: 콘솔 출력 코드페이지를 UTF-8로 설정, 입력은 ReadConsoleW(유니코드)로 읽고 UTF-8로 변환,
//             명령줄 인자도 UTF-16 → UTF-8로 변환
//  - 그 외 : 기본 동작 (이미 UTF-8)
void setupConsoleUtf8();
bool readLineUtf8(std::string& line);                    // EOF면 false
std::vector<std::string> utf8Args(int argc, char** argv);  // 프로그램 이름 제외

std::string executableDir(const char* argv0);               // 실행 파일이 있는 폴더 (못 찾으면 빈 문자열)

}  // namespace sc
