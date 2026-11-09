cmake_minimum_required(VERSION 3.16)
project(UsagiC VERSION 0.1.1 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(usagic src/main.c src/transpiler.c)
target_include_directories(usagic PRIVATE include)
if(MSVC)
    target_compile_options(usagic PRIVATE /W4 /utf-8)
else()
    target_compile_options(usagic PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_test(NAME version COMMAND $<TARGET_FILE:usagic> --version)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/examples/japanese.usagic"
        "${CMAKE_CURRENT_BINARY_DIR}/japanese.usagic" COPYONLY)
    set(JP_EXE "${CMAKE_CURRENT_BINARY_DIR}/usagic_japanese_sample${CMAKE_EXECUTABLE_SUFFIX}")
    add_test(NAME japanese_build COMMAND $<TARGET_FILE:usagic> build
        "${CMAKE_CURRENT_BINARY_DIR}/japanese.usagic"
        -o "${JP_EXE}" --cc "${CMAKE_C_COMPILER}" --force)
    set_tests_properties(japanese_build PROPERTIES FIXTURES_SETUP japanese_binary)
    add_test(NAME japanese_run COMMAND "${JP_EXE}")
    set_tests_properties(japanese_run PROPERTIES FIXTURES_REQUIRED japanese_binary
        PASS_REGULAR_EXPRESSION "count=0")
    if(UNIX)
        add_test(NAME integration COMMAND ${CMAKE_COMMAND} -E env
            "USAGIC_BIN=$<TARGET_FILE:usagic>" "TEST_CC=${CMAKE_C_COMPILER}"
            sh "${CMAKE_CURRENT_SOURCE_DIR}/tests/run.sh")
        set_tests_properties(integration PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endif()
