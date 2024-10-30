
/**
 * @file
 * @brief 排他的論理和を用いた転置式暗号を提供するライブラリです。
 */

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <threads.h>

/**
 * @brief 各種暗号関数で利用される鍵整数を保存するための型です。
 */

typedef size_t simple_xcypher_key;

/**
 * @brief 各種暗号関数が失敗した際に設定される値が定義された列挙型です。
 * @param SIMPLE_XCYPHER_ERRNO_NONE 関数は失敗しなかった。
 * @param SIMPLE_XCYPHER_ERRNO_INDEX_OUT_OF_RANGE データの読み出し位置が、データの末尾を超過した。
 */

typedef enum _simple_xcypher_errno {
  SIMPLE_XCYPHER_ERRNO_NONE = 0,
  SIMPLE_XCYPHER_ERRNO_INDEX_OUT_OF_RANGE
} _simple_xcypher_errno;

/**
 * @brief 各種暗号関数が失敗した際に設定される値が格納される大域変数です。
 * @note この大域変数は errno と同様にスレッドローカルに保存されます。
 */

extern thread_local _simple_xcypher_errno simple_xcypher_errno;

/**
 * @brief simple_xcypher_errno の値をメッセージ表示に適した文字列に変換します。
 * @param errno simple_xcypher_errno に設定された値です。
 * @return errno の内容にあった静的な文字列を返します。
 * @note 未定義の値が errno に指定された場合、この関数は `"Unknown errno given."` を返します。
 */

extern char __stdcall *simple_xcypher_errno_message (_simple_xcypher_errno errno);

/**
 * @brief 暗号化されたデータを保存する領域の大きさを求めます。
 * @param datasize 暗号化するデータの大きさです。
 * @return \f$datasize <= 2^n - 1\f$ の条件を満たす最小の非負整数を返します。
 * @note この関数は必ず成功します。
 */

extern size_t __stdcall simple_xcypher_calc_encrypted_data_size (size_t datasize);

/**
 * @brief 鍵を用いてデータを暗号化します。
 * @param data 暗号化するデータのポインタです。
 * @param datasize 暗号化するデータの大きさです。
 * @param key 暗号化に使われる任意の非負整数です。
 * @param encrypteddata 暗号化されたデータが書き込まれる領域のポインタです。
 * @param encrypteddatasize 暗号化されたデータが書き込まれる領域の大きさです。
 * @return この関数は必ず成功します。
 * @note encrypteddata, encrypteddatasize はそれぞれ simple_xcypher_calc_encrypted_data_size() 関数で計算された大きさ、値を指定することを強く推奨します。
 */

extern void __stdcall simple_xcypher_encrypt (const void *data, size_t datasize, simple_xcypher_key key, void *encrypteddata, size_t encrypteddatasize);

/**
 * @brief 鍵を用いてデータを部分的に復号します。
 * @param position 復号を開始する、平文を基準とした位置です。
 * @param size 復号を行う大きさです。
 * @param data 復号するデータのポインタです。
 * @param datasize 復号するデータの大きさです。
 * @param key 復号に使われる任意の非負整数です。
 * @param decrypteddata 復号されたデータが書き込まれる領域のポインタです。
 * @return 成功ならば 0 失敗ならば 1 を返します。
 * @warning decrypteddata の領域の大きさは size 以上でなければなりません。
 * @warning position, size の合計が datasize を超過する場合、メモリを保護するために、この関数は失敗します。
 */

extern int __stdcall simple_xcypher_decrypt (size_t position, size_t size, const void *data, size_t datasize, simple_xcypher_key key, void *decrypteddata);
