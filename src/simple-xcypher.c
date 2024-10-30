
#include <stddef.h>
#include <stdint.h>
#include <threads.h>
#include "simple-xcypher.h"

thread_local _simple_xcypher_errno simple_xcypher_errno = SIMPLE_XCYPHER_ERRNO_NONE;

char __stdcall *simple_xcypher_errno_message (_simple_xcypher_errno errno){
  switch (errno){
    case SIMPLE_XCYPHER_ERRNO_NONE:
      return "None.";
    case SIMPLE_XCYPHER_ERRNO_INDEX_OUT_OF_RANGE: 
      return "Index out of range.";
    default:
      return "Unknown errno given.";
  }
}

size_t __stdcall simple_xcypher_calc_encrypted_data_size (size_t datasize){
  if (0 < datasize){
    size_t n = 1;
    while (1){
      if (datasize <= n){
        break;
      }
      else {
        n <<= 1;
        n |= 1;
      }
    }
    return n;
  }
  else {
    return 0;
  }
}

static inline uint64_t xorlshift (uint64_t number){
  number ^= number << 13;
  number ^= number >> 7;
  number ^= number << 17;
  return number;
}

static inline void calc_index_and_mask (size_t index, simple_xcypher_key key, size_t encrypteddatasize, size_t *indexp, uint8_t *maskp){
  *indexp = (index ^ key) & encrypteddatasize;
  *maskp = xorlshift(index ^ key);
}

void __stdcall simple_xcypher_encrypt (const void *data, size_t datasize, simple_xcypher_key key, void *encrypteddata, size_t encrypteddatasize){
  for (size_t index = 0; index < datasize; index++){
    size_t index2;
    uint8_t mask;
    calc_index_and_mask(index, key, encrypteddatasize, &index2, &mask);
    ((uint8_t*)encrypteddata)[index2] = ((uint8_t*)data)[index] ^ mask;
  }
  for (size_t index = datasize; index < encrypteddatasize; index++){
    size_t index2;
    uint8_t mask;
    calc_index_and_mask(index, key, encrypteddatasize, &index2, &mask);
    ((uint8_t*)encrypteddata)[index2] = 0 ^ mask;
  }
}

int __stdcall simple_xcypher_decrypt (size_t position, size_t size, const void *data, size_t datasize, simple_xcypher_key key, void *decrypteddata){
  if (position + size <= datasize){
    for (size_t index = 0; index < size; index++){
      size_t index2;
      uint8_t mask;
      calc_index_and_mask(position + index, key, datasize, &index2, &mask);
      ((uint8_t*)decrypteddata)[index] = ((uint8_t*)data)[index2] ^ mask;
    }
    simple_xcypher_errno = SIMPLE_XCYPHER_ERRNO_NONE;
    return 0;
  }
  else {
    simple_xcypher_errno = SIMPLE_XCYPHER_ERRNO_INDEX_OUT_OF_RANGE;
    return 1;
  }
}
