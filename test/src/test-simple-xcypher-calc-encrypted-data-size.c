
#include <stddef.h>
#include <simple-xcypher/simple-xcypher.h>
#include "test.h"
#include "test-simple-xcypher-calc-encrypted-data-size.h"

static void testcase (){
  {
    size_t encrypteddatasize;
    TEST(simple_xcypher_calc_encrypted_data_size(0, &encrypteddatasize) == 0);
    TEST(encrypteddatasize == 0);
  }
  {
    size_t encrypteddatasize;
    TEST(simple_xcypher_calc_encrypted_data_size(1, &encrypteddatasize) == 0);
    TEST(encrypteddatasize == 2);
  }
  {
    size_t encrypteddatasize;
    TEST(simple_xcypher_calc_encrypted_data_size(0x80, &encrypteddatasize) == 0);
    TEST(encrypteddatasize == 0x100);
  }
  {
    size_t encrypteddatasize;
    TEST(simple_xcypher_calc_encrypted_data_size(SIZE_MAX, &encrypteddatasize) == 1);
  }
}

void test_simple_xcypher_calc_encrypted_data_size (){
  testcase();
}
