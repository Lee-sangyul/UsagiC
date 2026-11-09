# UsagiC v0.1.1-alpha keyword reference

Korean and Japanese keywords have equal status in v0.1. Established UsagiLang Japanese spellings are reused; new C-specific Japanese spellings are phonetic aliases. Korean and Japanese may be mixed in one .usagic file. The C output uses whole-token replacement, not substring replacement.

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


## Japanese and Korean complete equivalence table

| C | Japanese (日本語) | Korean (한국어) |
|---|---|---|
| short | ウク | 욱 |
| int | ウ | 우 |
| long | ウウ | 우우 |
| long long | ウウウウ | 우우우우 |
| float | ナ | 나 |
| double | ナナ | 나나 |
| long double | ウウナナ | 우우나나 |
| char | ヤ | 야 |
| void | プ | 푸 |
| bool | ハ | 하 |
| const | ツツ | 츠츠 |
| unsigned | ウヤ | 우야 |
| signed | ヤウ | 야우 |
| static | プルッ | 푸룻 |
| extern | ハウ | 하우 |
| volatile | プルプル | 푸루푸르 |
| inline | プヤ | 푸야 |
| restrict | ヨッ | 욧 |
| struct | ウラハ | 우라하 |
| typedef | プルヤハ | 푸루야하 |
| enum | ヤハヤ | 야하야 |
| union | ウラウ | 우라우 |
| sizeof | ウラハァ | 우라하아 |
| if | ハァ | 하아 |
| else | フゥン | 후웅 |
| while | プルルルル | 푸루루루루 |
| for | ツツウラウラ | 츠츠우라우라 |
| return | ウラララ | 우라라라 |
| break | サクッ | 사쿠 |
| continue | プワッ | 푸왓 |
| switch | ハァハァ | 하아하아 |
| case | ヤハウ | 야하우 |
| default | フゥンハ | 후웅하 |
| do | プルハ | 푸루하 |
| goto | ウラッ | 우랏 |
| puts | ウラ | 우라 |
| printf | ウラプルル | 우라푸르르 |
| scanf | ハァヤハウラ / ハイヤハウナ | 하아야하우라 / 하이야하우나 |
| true | ヤハ | 야하 |
| false | ウーラーヤハヤハ | 우나야하야하 |
| NULL | ナイナイ | 나이나이 |

| C preprocessor | Japanese (日本語) | Korean (한국어) |
|---|---|---|
| #include | #ハイ | #하이 |
| #define | #プルャ | #푸루야 |
| #if | #ハァ | #하아 |
| #ifdef | #ハァヤハ | #하아야하 |
| #ifndef | #ハァイヤ | #하아이야 |
| #elif | #フゥンハ | #후웅하 |
| #else | #フゥン | #후웅 |
| #endif | #サクッ | #사쿠 |
| #undef | #ウナ | #우나 |
| #error | #ヤダ | #야다 |
| #pragma | #プヤハ | #푸야하 |

Japanese-only and mixed-script compile tests live in examples/japanese.usagic and tests/run.sh. Existing standard C .h headers remain untranslated.
