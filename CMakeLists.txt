cmake_minimum_required(VERSION 3.16)
project(lunar_phases VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(LUNAR_ENABLE_SANITIZERS "Enable address and undefined-behavior sanitizers" OFF)
option(LUNAR_BUILD_CLI "Build the lunar_phases command-line executable" ON)

include(GNUInstallDirs)

if(LUNAR_ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(lunar_core
    src/calendar.c
    src/astronomy.c
    src/phases.c
)
target_include_directories(lunar_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/lunar_phases>
)
if(MSVC)
    target_compile_options(lunar_core PRIVATE /W4)
else()
    target_compile_options(lunar_core PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    target_link_libraries(lunar_core PUBLIC m)
endif()

if(LUNAR_BUILD_CLI)
    add_executable(lunar_phases src/main.c)
    target_link_libraries(lunar_phases PRIVATE lunar_core)
    if(MSVC)
        target_compile_options(lunar_phases PRIVATE /W4)
    else()
        target_compile_options(lunar_phases PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
    endif()
endif()

enable_testing()
add_executable(lunar_tests tests/test_lunar.c)
target_link_libraries(lunar_tests PRIVATE lunar_core)
if(MSVC)
    target_compile_options(lunar_tests PRIVATE /W4)
else()
    target_compile_options(lunar_tests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()
include(CTest)
if(BUILD_TESTING)
    add_test(NAME lunar_tests COMMAND lunar_tests)
endif()

include(CMakePackageConfigHelpers)

install(TARGETS lunar_core
    EXPORT LunarPhasesTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(LUNAR_BUILD_CLI)
    install(TARGETS lunar_phases RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunar_phases)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LunarPhasesConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LunarPhasesConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LunarPhasesConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LunarPhases"
)
install(EXPORT LunarPhasesTargets
    FILE LunarPhasesTargets.cmake
    NAMESPACE LunarPhases::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LunarPhases"
)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LunarPhasesConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LunarPhasesConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/LunarPhases"
)

