cmake_minimum_required(VERSION 3.14)
project(SpellCorreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(spellcorreter_lib STATIC ${LIB_SOURCES})
target_include_directories(spellcorreter_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
if(MSVC)
  target_compile_options(spellcorreter_lib PUBLIC /utf-8 /W4)
else()
  target_compile_options(spellcorreter_lib PUBLIC -Wall -Wextra)
endif()

if(WIN32)
  target_link_libraries(spellcorreter_lib PUBLIC shell32)
endif()

add_executable(spellcorreter app/main.cpp)
target_link_libraries(spellcorreter PRIVATE spellcorreter_lib)

# 빌드 후 영어 사전을 실행 파일 옆(dict/)에 복사
add_custom_command(TARGET spellcorreter POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:spellcorreter>/dict
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/dict/en_words.txt
          $<TARGET_FILE_DIR:spellcorreter>/dict/en_words.txt)

enable_testing()
add_executable(spellcorreter_tests tests/test_main.cpp)
target_link_libraries(spellcorreter_tests PRIVATE spellcorreter_lib)
add_test(NAME spellcorreter_tests COMMAND spellcorreter_tests)
