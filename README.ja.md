# UsagiC v0.1.1-alpha — 日本語ガイド

[English / 한국어](README.md) · [完全なキーワード表](docs/KEYWORDS.md)

**UsagiC は C11 で作られたネイティブ・トランスパイラです。** 韓国語と日本語のうさぎ風キーワードで、C 言語の構文を使ったプログラムを書けます。両言語のキーワードを同じ .usagic ファイル内で混ぜても動作します。

ソース .usagic → 標準 C → GCC / Clang → ネイティブ実行ファイル。Python は不要です。

> 『ちいかわ』の非公式ファンメイド作品です。原作者や権利者による承認・提携はありません。キーワードの意味は独自の言語仕様であり、公式翻訳ではありません。

## コンパイルと実行

GCC または Clang（C11 対応）が必要です。

~~~sh
make
./usagic --version
./usagic build examples/japanese.usagic
./examples/japanese
~~~

CMake でもビルドできます。

~~~sh
cmake -S . -B build
cmake --build build
~~~

## 日本語 Hello, World!

~~~c
#ハイ<stdio.h>

ウ main(プ) {
    ウラ("ヤハ！ Hello, World!");
    ウラララ 0;
}
~~~

## 基本データ型と修飾子

| C | 日本語 | 한국어 |
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

## 構造体と制御構文

| C | 日本語 | 한국어 |
|---|---|---|
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

## 入出力と値

| C | 日本語 | 한국어 |
|---|---|---|
| puts | ウラ | 우라 |
| printf | ウラプルル | 우라푸르르 |
| scanf | ハァヤハウラ / ハイヤハウナ | 하아야하우라 / 하이야하우나 |
| true | ヤハ | 야하 |
| false | ウーラーヤハヤハ | 우나야하야하 |
| NULL | ナイナイ | 나이나이 |

ウラ() は C の puts() と同じです（末尾に改行を付ける）。ウラプルル() は printf() なので、%d などのフォーマット指定子が使えます。
C11 の bool 型を使うときは stdbool.h を取り込んでください。
ハァヤハウラ() は UsagiC では C の scanf() に変換します（UsagiLang の独立した入力関数とは意味が異なります）。

## プリプロセッサ

| C | 日本語 | 한국어 |
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

**C の .h ヘッダーファイルはそのまま使います。** 例えば #ハイ<stdio.h> は #include<stdio.h> となります。ヘッダーファイルの内部は翻訳しません。

## 日本語 for ループ

~~~c
#ハイ<stdio.h>

ウ main(プ) {
    ツツウラウラ (ウ i = 0; i < 3; ++i) {
        ハァ (i == 1) {
            ウラ("ヤハ！");
        } フゥン {
            ウラプルル("%d\n", i);
        }
    }
    ウラララ 0;
}
~~~

C 言語の for (初期化; 条件; 更新) 形式を使います。独立言語 UsagiLang の for ループとは異なります。

## 追加のコマンド

~~~sh
./usagic transpile examples/japanese.usagic
./usagic build examples/japanese.usagic --cc clang
make test
~~~

文字列・文字リテラル・コメント・インクルードパスの中身は変換しません。v0.1 は完全な C パーサーではなく、C の構文や型の検証は GCC / Clang に任せます。信頼できない C コードの実行は避けてください。

MIT License · v0.1.1-alpha (experimental)

## v0.1.1-alpha 修正内容

- 日本語サンプル内の条件付きコンパイルを修正し、無効化されるべき #error が実行されなくなりました。
- UTF-8 BOM 付きの .usagic ソースを利用できます。
- 明示的な -o 出力先が既存ファイルの場合は標準で上書きを拒否します。必要な場合のみ --force を指定してください。
- GCC と Clang の双方で全テストを実行でき、CMake/CTest に日本語コンパイルと実行テストを登録しました。
- Visual Studio の開発者用コマンドプロンプトでは --cc cl または --cc msvc を指定できます。
- make clean で日本語サンプルの生成物も消去します。
- C ヘッダーは従来通り .h を使い、ヘッダー内部を翻訳しません。
