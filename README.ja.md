# UsagiC — v0.1.0-alpha

[English / 한국어 README](README.md) · [キーワード一覧](docs/KEYWORDS.md)

UsagiC は『ちいかわ』のうさぎの表現に着想を得た **C11 製のトランスパイラ**です。.usagic ソースを C に変換して GCC / Clang でネイティブ実行ファイルを生成します。Python は不要です。

> 非公式ファンメイド作品です。原作者・権利者との提携や承認はありません。キーワードの意味は独自に定義したプログラミング上の約束であり、公式な翻訳ではありません。

## ビルドと実行

~~~sh
make
./usagic build examples/hello.usagic
./examples/hello
~~~

CMake も利用できます。

~~~sh
cmake -S . -B build
cmake --build build
~~~

## コード例

~~~c
#하이<stdio.h>

우 main(푸) {
    우라("ヤハ！ Hello, World!");
    우라라라 0;
}
~~~

| C | UsagiC | C | UsagiC |
|---|---|---|---|
| int | 우 | void | 푸 |
| if | 하아 / ハァ | else | 후웅 / フゥン |
| while | 푸루루루루 / プルルルル | for | 츠츠우라우라 / ツツウラウラ |
| return | 우라라라 / ウラララ | puts | 우라 / ウラ |
| printf | 우라푸르르 | #include | #하이 / #ハイ |

[全キーワード一覧](docs/KEYWORDS.md)も参照してください。

#하이<stdio.h> は標準 C の #include<stdio.h> に変換します。.h ヘッダーの中身は変換しません。우라 は puts と同様に改行を追加し、우라푸르르 は printf と同じ書式指定を使います。C11 で bool 型を使う場合は stdbool.h を取り込んでください。

~~~sh
./usagic transpile examples/hello.usagic
./usagic build examples/hello.usagic --cc clang
make test
~~~

MIT License. アルファ版の実験的プロジェクトです。信頼できないネイティブコードを実行しないでください。
