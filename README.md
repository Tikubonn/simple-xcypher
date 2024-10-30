
# simple-xcypher

![](https://img.shields.io/badge/-C++-blue?logo=cplusplus)
![](https://img.shields.io/badge/license-MIT-blue)
![](https://img.shields.io/badge/version-1.0.0-orange)

simple-xcypher は排他的論理和を用いた転置式暗号を提供します。

提供される暗号機能は次のような特徴を有しています。

* ブロック暗号です。
* 共通鍵暗号方式を採用しています。
* ブロックの大きさに `2^n - 1` の条件を満たす非負整数を用います。 
* 部分的な復号が可能です(ランダムアクセス)。
* 転置式暗号なので、所定の位置に必ず出現する文章があっても、そこを足掛かりに復号されてしまう危険性が低くなります。
* コストの低い演算が使われているので、高速に動作します。

提供される暗号機能で留意すべき点は次のとおりです。

* ブロックの大きさに２の累乗を基準とした整数を用いるため、パディング領域が大きくなる場合があります。
* 当然ですが、これも同梱されている鍵を抜き取られてしまうと、正規の手順で復号されてしまいます。

## Details 

simple-xcypher の転置先の位置計算は単純明快です。

```
任意の非負整数 A, K, i と A <= 2^n -1 の条件を満たす非負整数 B があるとする。
i の取りうる値が i < A のとき i XOR K AND B から得られる値はそれぞれ重複しない。
```

ただし `K = 0` の場合には転置が行われないので `0 < K` を満たすように `K` の値を設定する必要があります。

<small>※単純な転置操作だけでは暗号強度が脆弱なので、本ライブラリでは転置操作に加えて、各バイト毎に Xorlshift とバーナム暗号を組み合わせた暗号化が追加で施されています。</small>

## Usage

ビルド後に作成される `dist/include` `dist/lib` 内のファイルを適切な場所に配置して使います。 それ以外は標準的なC言語ライブラリと同じように扱います。

```c
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <simple-xcypher/simple-xcypher.h>

#define PLAINTEXT "This is plaintext!"

int main (){

  size_t encrypteddatasize = simple_xcypher_calc_encrypted_data_size(sizeof(PLAINTEXT));

  uint8_t encrypteddata[encrypteddatasize];
  simple_xcypher_encrypt(PLAINTEXT, sizeof(PLAINTEXT), 0x123, encrypteddata, encrypteddatasize);
  printf("encrypted = ");
  fwrite(encrypteddata, encrypteddatasize, 1, stdout);
  putc('\n', stdout);

  uint8_t decrypteddata[sizeof(PLAINTEXT)];
  simple_xcypher_decrypt(0, sizeof(PLAINTEXT), encrypteddata, encrypteddatasize, 0x123, decrypteddata);
  printf("decrypted = %s\n", decrypteddata);

  return 0;

}
```

```bash
gcc -Idist/include -Ldist/lib example.c -lsimplexcypher
```

## Command 

ビルド後に作成される `bin/simple-xcypher.exe` を実行することで、コマンドラインから暗号機能を利用することができます。

```bash
./bin/simple-xcypher -k 0x123
```

## Build 

同梱されている `makefile` を使用することでビルドすることができます。

```bash
make
```

非常に簡素なテストも同梱されています。

```bash
make test
```

## License 

&copy; 2024 tikubonn

simple-xcypher released under the [MIT License](./LICENSE).
