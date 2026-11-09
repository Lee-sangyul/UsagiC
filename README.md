# UsagiC 🐰 — v0.1.1-alpha

[日本語 README](README.ja.md) · [Full keyword reference](docs/KEYWORDS.md)

**UsagiC is a C-style esoteric programming language inspired by Usagi from Chiikawa.** It has a native **C11 transpiler implemented in C**. It converts .usagic source into standard C, then invokes GCC or Clang to produce an executable. Python is not required.

> Unofficial fan project. Not affiliated with or endorsed by Chiikawa's original creators or rights holders. Keyword meanings are original programming conventions, not official translations. No character artwork or audio is included.

## Build

Prerequisites: a C11 compiler (GCC or Clang), make or CMake, and a UTF-8 editor.

~~~sh
make
./usagic build examples/hello.usagic
./examples/hello
~~~

Or build the tool with CMake:

~~~sh
cmake -S . -B build
cmake --build build
~~~

## Hello, World!

~~~c
#하이<stdio.h>

우 main(푸) {
    우라("ヤハ！ Hello, World!");
    우라라라 0;
}
~~~

## CLI

~~~sh
./usagic --help
./usagic --version
./usagic transpile examples/hello.usagic
./usagic transpile examples/hello.usagic -o hello.c
./usagic build examples/hello.usagic
./usagic build examples/hello.usagic -o hello --cc clang
~~~

- Transpile defaults to a generated C file named <input>.c, e.g. hello.usagic.c.
- Build also generates <input>.c, keeps it for inspection, and outputs a native executable.
- GCC/Clang errors can reference the original .usagic filename via generated #line directives.
- The -o option selects C output in transpile mode and executable output in build mode.
- Source size is limited to 16 MiB in this alpha.

## Syntax

UsagiC follows C syntax for parentheses, braces, semicolons, arrays and pointers. It uses Usagi-style reserved words for C types and control flow. Headers stay ordinary .h files and are **never translated**.

| C | UsagiC | C | UsagiC |
|---|---|---|---|
| int | 우 | short | 욱 |
| long | 우우 | long long | 우우우우 |
| float | 나 | double | 나나 |
| long double | 우우나나 | char | 야 |
| void | 푸 | bool | 하 |
| const | 츠츠 | if | 하아 |
| else | 후웅 | while | 푸루루루루 |
| for | 츠츠우라우라 | return | 우라라라 |
| puts | 우라 | printf | 우라푸르르 |
| scanf | 하이야하우나 | #include | #하이 |

C headers and C standard-library functions such as malloc, free and strlen are left unchanged. Every UsagiC keyword has a Korean and a Japanese spelling, and both scripts can be mixed in one source file. UsagiLang-compatible commands preserve their existing Japanese names.

**Important:** In C11, bool, true and false require the standard header stdbool.h. UsagiC's 우라 calls puts (one string and an automatic newline), whereas 우라푸르르 calls printf (format-string arguments). 하이야하우나 maps to scanf, *not* the independent UsagiLang input helper.

The lexer preserves comments, strings, character literals and include paths rather than replacing words inside them. This v0.1 implementation does not implement a full C parser or semantic analyzer: GCC/Clang handles native C rules and error checking. Standard C identifiers matching reserved UsagiC keywords cannot be used unchanged.

See [docs/KEYWORDS.md](docs/KEYWORDS.md) for all types, modifiers, structs, branches, macros and aliases.

## Tests and licensing

~~~sh
make test
ctest --test-dir build --output-on-failure
~~~

The repository includes compile-and-run examples, lexer tests, CMake and GCC/Clang CI. MIT licensed. This is experimental software; do not execute untrusted native C code.

UsagiC is part of the **Usagi Language Family**, related to but separate from [UsagiLang](https://github.com/Lee-sangyul/UsagiLang).

## Japanese syntax / 日本語構文

Japanese spellings work in the transpiler, not only in documentation:

~~~c
#ハイ<stdio.h>

ウ main(プ) {
    ウラ("ヤハ！");
    ウラプルル("数: %d\n", 42);
    ハァ (42 > 0) { ウラ("OK"); }
    ウラララ 0;
}
~~~

Try the complete Japanese-only test program:

~~~sh
./usagic build examples/japanese.usagic
./examples/japanese
make test
~~~

The Japanese syntax is documented in [README.ja.md](README.ja.md). C headers still use their ordinary .h extension and are never rewritten.

## v0.1.1-alpha reliability fixes

- Fixed the Japanese sample's inverted preprocessor condition so its deliberately disabled #error does not trigger.
- Input files may have an optional UTF-8 BOM, which is removed before tokenization.
- Explicit -o output paths are protected from overwrites by default; use --force when deliberate.
- Generated .usagic.c outputs with a valid generator header, and standard executable build outputs, can be regenerated.
- GCC and Clang each run all integration tests using isolated temporary files.
- CMake/CTest includes Japanese compilation and execution, plus full shell tests on Unix.
- Windows support: cl.exe with native MSVC C11 flags and UTF-8 input, selected by --cc cl (or --cc msvc) from a Visual Studio Developer Prompt.
- make clean now removes all supported sample executables, including the Japanese example.

### Linux and macOS testing

~~~sh
make clean && make CC=gcc test
make clean && make CC=clang test
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
~~~

### Windows (Visual Studio Developer Command Prompt)

~~~powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
~~~

To avoid accidental data loss, --force never permits overwriting the original .usagic source itself. Headers continue to use ordinary .h files, unchanged.
