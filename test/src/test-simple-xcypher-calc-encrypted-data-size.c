
#include <stddef.h>
#include <simple-xcypher/simple-xcypher.h>
#include "test.h"
#include "test-simple-xcypher-calc-encrypted-data-size.h"

static void testcase (){
  TEST(simple_xcypher_calc_encrypted_data_size(0) == 0, "");
  TEST(simple_xcypher_calc_encrypted_data_size(1) == 1, "");
  TEST(simple_xcypher_calc_encrypted_data_size(0x80) == 0xff, "");
  TEST(simple_xcypher_calc_encrypted_data_size(SIZE_MAX) == SIZE_MAX, "");
}

void test_simple_xcypher_calc_encrypted_data_size (){
  testcase();
}
