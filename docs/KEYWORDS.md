# UsagiC v0.1.0-alpha keyword reference

The **Korean spellings** are the primary spelling for v0.1. Existing UsagiLang Japanese spellings are additionally accepted where listed. The C output uses whole-token replacement, not substring replacement.

## Types and qualifiers

| C output | UsagiC |
|---|---|
| short | 욱 |
| int | 우 |
| long | 우우 |
| long long | 우우우우 |
| float | 나 |
| double | 나나 |
| long double | 우우나나 |
| char | 야 |
| void | 푸 |
| bool | 하 |
| const | 츠츠 |
| unsigned | 우야 |
| signed | 야우 |
| static | 푸룻 |
| extern | 하우 |
| volatile | 푸루푸르 |
| inline | 푸야 |
| restrict | 욧 |

The combination 우야 우우우우 becomes unsigned long long. Underlying C type widths and ABI depend on your C compiler and platform.

## Aggregate types, operators and control flow

| C output | UsagiC | Japanese alias |
|---|---|---|
| struct | 우라하 | — |
| typedef | 푸루야하 | — |
| enum | 야하야 | — |
| union | 우라우 | — |
| sizeof | 우라하아 | — |
| if | 하아 | ハァ |
| else | 후웅 | フゥン |
| while | 푸루루루루 / 푸루루루 | プルルルル |
| for | 츠츠우라우라 | ツツウラウラ |
| return | 우라라라 | ウラララ |
| break | 사쿠 | サクッ |
| continue | 푸왓 | プワッ |
| switch | 하아하아 | — |
| case | 야하우 | — |
| default | 후웅하 | — |
| do | 푸루하 | — |
| goto | 우랏 | — |

UsagiC's for loop follows C syntax (initializer; condition; step), not the iterable-based UsagiLang for semantics.

## Output and common constants

| C symbol/function | UsagiC | Japanese alias |
|---|---|---|
| puts() | 우라() | ウラ() |
| printf() | 우라푸르르() | — |
| scanf() | 하이야하우나() / 하아야하우라() | ハァヤハウラ() |
| true | 야하 | ヤハ |
| false | 우나야하야하 / 우라야하야하 | ウーラーヤハヤハ |
| NULL | 나이나이 | ナイナイ |

All these are ordinary C symbols and require the usual C headers. In C11, include stdbool.h to use the bool/true/false macros.

## Preprocessor directives

| C directive | UsagiC |
|---|---|
| #include | #하이 or #ハイ |
| #define | #푸루야 |
| #if | #하아 |
| #ifdef | #하아야하 |
| #ifndef | #하아이야 |
| #elif | #후웅하 |
| #else | #후웅 |
| #endif | #사쿠 |
| #undef | #우나 |
| #error | #야다 |
| #pragma | #푸야하 |

Header names and content are preserved: #하이<stdio.h> maps to #include<stdio.h> and #하이"my_header.h" maps to #include"my_header.h". Ordinary .h files are not translated: write standard C in them.

## Current limitations

- C11 is the baseline. Native C headers, pointers, arrays, macros, standard library functions and C type semantics are delegated to the installed compiler.
- Original UsagiLang try/catch keywords are not available in standard C11, so they are intentionally excluded in v0.1.
- v0.1 is a lexer-based transpiler, not a complete C parser, type checker or sandbox.
- UTF-8 Japanese and Korean keywords can be used in source, but Windows compiler console/encoding configuration may vary.
