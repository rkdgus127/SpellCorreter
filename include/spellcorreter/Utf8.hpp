#pragma once
#include <string>

namespace sc::text {
// UTF-8 <-> UTF-32 변환 (코드포인트 단위로 다루기 위함)
std::u32string toU32(const std::string& s);
std::string toU8(const std::u32string& s);
}  // namespace sc::text
