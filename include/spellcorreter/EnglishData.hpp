#pragma once
#include <string>
#include <vector>

// 영어 내장 규칙 데이터 (오타표 / 구 규칙 / 축약형). 로직 없이 데이터만 둔다.
namespace sc::en {

struct Typo   { const char* wrong; const char* right; const char* msg; };
struct Phrase { const char* from;  const char* to;    const char* msg; };

const std::vector<Typo>& typos();
const std::vector<Phrase>& phrases();
const std::vector<const char*>& contractions();

}  // namespace sc::en
