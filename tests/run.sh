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

echo 'All UsagiC v0.1 tests passed.'
