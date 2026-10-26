cmake_minimum_required(VERSION 3.16)
project(lunar_phases C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(LUNAR_ENABLE_SANITIZERS "Enable address and undefined-behavior sanitizers" OFF)

if(LUNAR_ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(lunar_core
    src/calendar.c
    src/astronomy.c
    src/phases.c
)
target_include_directories(lunar_core PUBLIC include)
target_compile_options(lunar_core PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
target_link_libraries(lunar_core PUBLIC m)

add_executable(lunar_phases src/main.c)
target_link_libraries(lunar_phases PRIVATE lunar_core)
target_compile_options(lunar_phases PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

enable_testing()
add_executable(lunar_tests tests/test_lunar.c)
target_link_libraries(lunar_tests PRIVATE lunar_core)
target_compile_options(lunar_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
include(CTest)
if(BUILD_TESTING)
    add_test(NAME lunar_tests COMMAND lunar_tests)
endif()

include(GNUInstallDirs)
install(TARGETS lunar_phases RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunar_phases)

