
#include <stddef.h>
#include <stdint.h>
#include <simple-xcypher/simple-xcypher.h>
#include "test.h"
#include "test-simple-xcypher-encrypt-decrypt.h"

static void testcase (){
  uint8_t plaintext[256] = {0};
  size_t encrypteddatasize = simple_xcypher_calc_encrypted_data_size(sizeof(plaintext));
  TEST(encrypteddatasize == 511, "");
  uint8_t encrypteddata[encrypteddatasize];
  const simple_xcypher_key key = 123;
  simple_xcypher_encrypt(plaintext, sizeof(plaintext), key, encrypteddata, encrypteddatasize);
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(plaintext); index++){
    samecount += plaintext[index] == encrypteddata[index];
  }
  TEST(samecount < 256, "");

  //decrypt full.

  {
    uint8_t decrypteddata[sizeof(encrypteddata)];
    TEST(simple_xcypher_decrypt(0, sizeof(encrypteddata), encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < sizeof(plaintext); index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
    for (size_t index = sizeof(plaintext); index < sizeof(decrypteddata); index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt partially.

  {
    uint8_t decrypteddata[128];
    TEST(simple_xcypher_decrypt(0, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially2.

  {
    uint8_t decrypteddata[128];
    TEST(simple_xcypher_decrypt(128, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index +128], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially3.

  {
    uint8_t decrypteddata[1];
    TEST(simple_xcypher_decrypt(256, 255, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 255; index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt (always error).

  {
    uint8_t decrypteddata[1024];
    TEST(simple_xcypher_decrypt(0, 1024, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
  }

  //decrypt (always error2).

  {
    uint8_t decrypteddata[1];
    TEST(simple_xcypher_decrypt(1024, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
  }
}

static void testcase2 (){
  uint8_t plaintext[256];
  for (size_t index = 0; index < sizeof(plaintext); index++){
    plaintext[index] = index & 0xff;
  }
  size_t encrypteddatasize = simple_xcypher_calc_encrypted_data_size(sizeof(plaintext));
  TEST(encrypteddatasize == 511, "");
  uint8_t encrypteddata[encrypteddatasize];
  const simple_xcypher_key key = 123;
  simple_xcypher_encrypt(plaintext, sizeof(plaintext), key, encrypteddata, encrypteddatasize);
  size_t samecount = 0;
  for (size_t index = 0; index < sizeof(plaintext); index++){
    samecount += plaintext[index] == encrypteddata[index];
  }
  TEST(samecount < 256, "");

  //decrypt full.

  {
    uint8_t decrypteddata[sizeof(encrypteddata)];
    TEST(simple_xcypher_decrypt(0, sizeof(encrypteddata), encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < sizeof(plaintext); index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
    for (size_t index = sizeof(plaintext); index < sizeof(decrypteddata); index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt partially.

  {
    uint8_t decrypteddata[128];
    TEST(simple_xcypher_decrypt(0, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially2.

  {
    uint8_t decrypteddata[128];
    TEST(simple_xcypher_decrypt(128, 128, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 128; index++){
      TEST(decrypteddata[index] == plaintext[index +128], "0x%02x == 0x%02x", plaintext[index], decrypteddata[index]);
    }
  }

  //decrypt partially3.

  {
    uint8_t decrypteddata[1];
    TEST(simple_xcypher_decrypt(256, 255, encrypteddata, sizeof(encrypteddata), key, decrypteddata) == 0, "");
    for (size_t index = 0; index < 255; index++){
      TEST(decrypteddata[index] == 0, "0x%02x", decrypteddata[index]);
    }
  }

  //decrypt (always error).

  {
    uint8_t decrypteddata[1024];
    TEST(simple_xcypher_decrypt(0, 1024, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
  }

  //decrypt (always error2).

  {
    uint8_t decrypteddata[1];
    TEST(simple_xcypher_decrypt(1024, 1, encrypteddata, sizeof(encrypteddata), key, decrypteddata) != 0, "");
  }
}

void test_simple_xcypher_encrypt_decrypt (){
  testcase();
  testcase2();
}