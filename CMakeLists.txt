# https://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
# Commands:
## Generate compile_commands.json file
# - cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build

## Actually build or just use other script
# - cmake --build build

cmake_minimum_required(VERSION 3.10)
project(HandmadeHero)
add_executable(HandmadeHero WIN32 ./src/win32_handmade.cpp)
target_link_libraries(HandmadeHero user32.lib Gdi32.lib)
