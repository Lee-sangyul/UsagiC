#!/bin/sh
set -eu
BIN=./usagic
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

for sample in hello variables loops structs; do
  "$BIN" build "examples/$sample.usagic" -o "$TMP/$sample" >/dev/null
  "$TMP/$sample" > "$TMP/$sample.out"
done

grep -F 'ヤハ！ Hello, World!' "$TMP/hello.out" >/dev/null
grep -F '우라 remains text' "$TMP/variables.out" >/dev/null
printf '0\n1\n3\ndone\n' > "$TMP/expected"
diff -u "$TMP/expected" "$TMP/loops.out"
grep -F 'Hello, Usagi!' "$TMP/structs.out" >/dev/null

cat > "$TMP/lex.usagic" <<'SAMPLE'
#하이<stdio.h>
// 우라 우우 if 나나 should not be replaced
/* 하아 "우라" stays untouched */
우 main(푸) {
    츠츠 야 *s = "우라 하아 우우나나";
    야 c = 'x';
    우라(s);
    우라라라 0;
}
SAMPLE
"$BIN" transpile "$TMP/lex.usagic" -o "$TMP/lex.c" >/dev/null
grep -F '/* 하아 "우라" stays untouched */' "$TMP/lex.c" >/dev/null
grep -F '"우라 하아 우우나나"' "$TMP/lex.c" >/dev/null
cc -std=c11 "$TMP/lex.c" -o "$TMP/lex"
"$TMP/lex" > "$TMP/lex.out"
grep -F '우라 하아 우우나나' "$TMP/lex.out" >/dev/null

cat > "$TMP/bad.usagic" <<'SAMPLE'
#하이<stdio.h>
우 main(푸) {
  우라("missing end);
}
SAMPLE
if "$BIN" transpile "$TMP/bad.usagic" -o "$TMP/bad.c" > "$TMP/error.out" 2>&1; then
  echo 'FAIL: expected unterminated literal to fail' >&2; exit 1
fi
grep -F 'unterminated literal' "$TMP/error.out" >/dev/null


# Japanese-only program (compile AND execute)
"$BIN" build examples/japanese.usagic -o "$TMP/japanese" >/dev/null
"$TMP/japanese" > "$TMP/japanese.out"
grep -F 'こんにちは、うさぎ！' "$TMP/japanese.out" >/dev/null
grep -F 'count=0' "$TMP/japanese.out" >/dev/null
grep -F 'count=2' "$TMP/japanese.out" >/dev/null
grep -F 'ヤハ！' "$TMP/japanese.out" >/dev/null
grep -F 'long long=999, long double=1.25' "$TMP/japanese.out" >/dev/null

# Whole-token aliases + Japanese preprocessor aliases
cat > "$TMP/keywords.usagic" <<'JAPANESE_TOKENS'
#ハイ<stdio.h>
#プルャ VALUE 1
#ハァ VALUE
#ハァヤハ VALUE
#フゥンハ VALUE == 0
#フゥン
#サクッ
#サクッ
#ハァイヤ UNKNOWN
#ヤダ should be skipped
#サクッ
#ウナ VALUE
#プヤハ once
ウ ウク ウウ ウウウウ ナ ナナ ウウナナ
ヤ プ ハ ツツ ウヤ ヤウ プルッ ハウ プルプル プヤ ヨッ
ウラハ プルヤハ ヤハヤ ウラウ ウラハァ
ハァハァ ヤハウ フゥンハ プルハ ウラッ ウラプルル ハイヤハウナ
ヤハ ウナヤハヤハ ウラヤハヤハ ナイナイ
JAPANESE_TOKENS
"$BIN" transpile "$TMP/keywords.usagic" -o "$TMP/keywords.c" >/dev/null
grep -F '#include<stdio.h>' "$TMP/keywords.c" >/dev/null
grep -F '#define VALUE 1' "$TMP/keywords.c" >/dev/null
grep -F '#ifdef VALUE' "$TMP/keywords.c" >/dev/null
grep -F '#ifndef UNKNOWN' "$TMP/keywords.c" >/dev/null
grep -F '#elif VALUE == 0' "$TMP/keywords.c" >/dev/null
grep -F '#undef VALUE' "$TMP/keywords.c" >/dev/null
grep -F '#pragma once' "$TMP/keywords.c" >/dev/null
cat > "$TMP/expected-keywords" <<'EXPECTED_TOKENS'
int short long long long float double long double
char void bool const unsigned signed static extern volatile inline restrict
struct typedef enum union sizeof
switch case default do goto printf scanf
true false false NULL
EXPECTED_TOKENS
grep -E '^(int short|char void|struct typedef|switch case|true false)' "$TMP/keywords.c" > "$TMP/actual-keywords"
diff -u "$TMP/expected-keywords" "$TMP/actual-keywords"

# Korean and Japanese keywords mixed in the same file
cat > "$TMP/mixed.usagic" <<'MIXED_SOURCE'
#ハイ<stdio.h>
// ウラ should remain unchanged in comments
ウ main(푸) {
    ツツ ヤ *message = "ウラ 우라 ハァ 하아";
    우라(message);
    ウラララ 0;
}
MIXED_SOURCE
"$BIN" build "$TMP/mixed.usagic" -o "$TMP/mixed" >/dev/null
"$TMP/mixed" > "$TMP/mixed.out"
grep -Fx 'ウラ 우라 ハァ 하아' "$TMP/mixed.out" >/dev/null
grep -F '// ウラ should remain unchanged' "$TMP/mixed.usagic.c" >/dev/null

echo 'All UsagiC v0.1 tests passed (Korean + Japanese).'
