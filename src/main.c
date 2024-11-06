
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <simple-xcypher/simple-xcypher.h>
#include "buffer.h"

typedef enum mode {
  MODE_ENCRYPTION,
  MODE_DECRYPTION
} mode;

static int parse_uint_as_hexadecimal (char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 16;
      value += str[index] - '0';
    }
    else 
    if ('a' <= str[index] && str[index] <= 'f'){
      value *= 16;
      value += str[index] - 'a' + 16;
    }
    else 
    if ('A' <= str[index] && str[index] <= 'F'){
      value *= 16;
      value += str[index] - 'A' + 16;
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing hexadecimal.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static int parse_uint_as_digits (char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 10;
      value += str[index] - '0';
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing digits.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static int parse_uint (char *str, uintmax_t *valuep){
  if (str[0] == '0'){
    if (str[1] == 'x'){
      return parse_uint_as_hexadecimal(str +2, valuep);
    }
    else {
      return parse_uint_as_digits(str, valuep);
    }
  }
  else {
    return parse_uint_as_digits(str, valuep);
  }
}

static int parse_args (int argc, char **argv, mode *modep, FILE **inputp, FILE **outputp, size_t *positionp, size_t *sizep, bool *givensizep, simple_xcypher_key *keyp, bool *showhelpp, bool *showversionp){
  mode mode = MODE_ENCRYPTION;
  char *inputfile = NULL;
  char *outputfile = NULL;
  uintmax_t position = 0;
  uintmax_t size;
  uintmax_t key;
  bool givenmode = false;
  bool givenposition = false;
  bool givensize = false;
  bool givenkey = false;
  bool showhelp = false; 
  bool showversion = false;
  size_t index = 1;
  while (index < argc){
    if (strcmp(argv[index], "--help") == 0){
      showhelp = true;
      break;
    }
    else 
    if (strcmp(argv[index], "--version") == 0){
      showversion = true;
      break;
    }
    else 
    if (strcmp(argv[index], "-e") == 0){
      if (!givenmode){
        mode = MODE_ENCRYPTION;
        givenmode = true;
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a parameter -e or -d.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-d") == 0){
      if (!givenmode){
        mode = MODE_DECRYPTION;
        givenmode = true;
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a parameter -d or -d.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-p") == 0){
      if (index +1 < argc){
        if (!givenposition){
          if (parse_uint(argv[index +1], &position)){
            return 1;
          }
          givenposition = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -p %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -p.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-s") == 0){
      if (index +1 < argc){
        if (!givensize){
          if (parse_uint(argv[index +1], &size)){
            return 1;
          }
          givensize = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -s %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -s.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-k") == 0){
      if (index +1 < argc){
        if (!givenkey){
          if (parse_uint(argv[index +1], &key)){
            return 1;
          }
          givenkey = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -k %s.\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -k.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-o") == 0){
      if (index +1 < argc){
        if (outputfile == NULL){
          outputfile = argv[index +1];
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a output file \"%s\".\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -o.\n");
        return 1;
      }
    }
    else {
      if (inputfile == NULL){
        inputfile = argv[index];
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a input file \"%s\".\n", argv[index]);
        return 1;
      }
    }
  }
  if (showhelp){
    *modep = MODE_ENCRYPTION;
    *inputp = stdin;
    *outputp = stdout;
    *positionp = 0;
    *sizep = 0;
    *givensizep = false;
    *keyp = 0;
    *showhelpp = true;
    *showversionp = false;
    return 0;
  }
  else 
  if (showversion){
    *modep = MODE_ENCRYPTION;
    *inputp = stdin;
    *outputp = stdout;
    *positionp = 0;
    *sizep = 0;
    *givensizep = false;
    *keyp = 0;
    *showhelpp = false;
    *showversionp = true;
    return 0;
  }
  else {
    if (givenkey){
      FILE *input;
      if (inputfile == NULL){
        input = stdin;
      }
      else {
        input = fopen(inputfile, "rb");
        if (input == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", inputfile, errno);
          return 1;
        }
      }
      FILE *output;
      if (outputfile == NULL){
        output = stdout;
      }
      else {
        output = fopen(outputfile, "wb");
        if (output == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", outputfile, errno);
          return 1;
        }
      }
      *modep = mode;
      *inputp = input;
      *outputp = output;
      *positionp = position;
      *sizep = size;
      *givensizep = givensize;
      *keyp = key;
      *showhelpp = false;
      *showversionp = false;
      return 0;
    }
    else {
      fprintf(stderr, "Require a -k parameter.\n");
      return 1;
    }
  }
}

static void *read_all_input (FILE *input, size_t *writtensizep){
  buffer buffer;
  buffer_setup(&buffer);
  while (true){
    char data[4096];
    size_t datasize = fread(data, 1, sizeof(data), input);
    if (0 < datasize){
      if (buffer_write(data, datasize, &buffer)){
        fprintf(stderr, "Caused some error at writing to buffer (errno = %d).\n", errno);
        return NULL;
      }
    }
    if (datasize < sizeof(data)){
      if (ferror(input)){
        fprintf(stderr, "Caused some error at reading from file (errno = %d).\n", errno);
        return NULL;
      }
      else {
        break;
      }
    }
  }
  uint8_t *bufferdata;
  buffer_data(&buffer, &bufferdata, writtensizep);
  return bufferdata;
}

static int encrypt (simple_xcypher_key key, FILE *input, FILE *output){
  size_t datasize;
  void *data = read_all_input(input, &datasize);
  if (data == NULL){
    return 1;
  }
  size_t encrypteddatasize;
  if (simple_xcypher_calc_encrypted_data_size(datasize, &encrypteddatasize)){
    fprintf(stderr, "Plain text is too much large (size = %zu).\n", datasize);
    return 1;
  }
  uint8_t *encrypteddata = malloc(encrypteddatasize);
  if (encrypteddata == NULL){
    fprintf(stderr, "Could not allocate memory (size = %zu, errno = %d).\n", encrypteddatasize, errno);
    return 1;
  }
  simple_xcypher_encrypt(data, datasize, key, encrypteddata, encrypteddatasize);
  size_t writtensize = fwrite(encrypteddata, 1, encrypteddatasize, output);
  if (writtensize < encrypteddatasize){
    fprintf(stderr, "Caused some error at writing into a file (errno = %d).\n", errno);
    return 1;
  }
  return 0;
}

static int decrypt (size_t position, size_t size, bool givensize, simple_xcypher_key key, FILE *input, FILE *output){
  size_t datasize;
  void *data = read_all_input(input, &datasize);
  if (data == NULL){
    return 1;
  }
  size_t decrypteddatasize = givensize? size: datasize;
  void *decrypteddata = malloc(decrypteddatasize);
  if (decrypteddata == NULL){
    fprintf(stderr, "Could not allocate memory (size = %zu).\n", decrypteddatasize);
    return 1;
  }
  if (simple_xcypher_decrypt(position, decrypteddatasize, data, datasize, key, decrypteddata)){
    char *message = simple_xcypher_errno_message(simple_xcypher_errno);
    fprintf(stderr, "Could not decrypt (simple_xcypher_errno = %d, %s).\n", simple_xcypher_errno, message);
    return 1;
  }
  size_t writtensize = fwrite(decrypteddata, 1, decrypteddatasize, output);
  if (writtensize < decrypteddatasize){
    fprintf(stderr, "Caused some error at writing into a file.\n");
    return 1;
  }
  return 0;
}

const char HELP_MESSAGE[] = 
"Usage: simple-xcypher [OPTION]... -k KEY [FILE]\n"
"Encrypt or decrypt FILE with simple-xcypher.\n"
"\n"
"OPTION:\n"
"  -o file: An output file. default is stdout.\n"
"  -p position: An position to read start. It use to decryption mode.\n"
"  -s size: An read size. It use to decryption mode.\n"
"  --help: Show help then exit.\n"
"  --version: Show version then exit.\n"
"KEY: An unsigned integer used to common key. You can use digits or hexadecimal format like as 123 and 0x123abc.\n"
"FILE: An input file. default is stdin.\n"
;

const char VERSION_MESSAGE[] = 
"simple-xcypher 1.0.0\n"
;

int main (int argc, char **argv){
  mode mode;
  FILE *input;
  FILE *output;
  size_t position;
  size_t size;
  bool givensize;
  simple_xcypher_key key;
  bool showhelp;
  bool showversion;
  if (parse_args(argc, argv, &mode, &input, &output, &position, &size, &givensize, &key, &showhelp, &showversion)){
    return 1;
  }
  if (showhelp){
    printf("%s", HELP_MESSAGE);
    return 0;
  }
  else 
  if (showversion){
    printf("%s", VERSION_MESSAGE);
    return 0;
  }
  else {
    switch (mode){
      case MODE_ENCRYPTION:
        return encrypt(key, input, output);
      case MODE_DECRYPTION: 
        return decrypt(position, size, givensize, key, input, output);
      default:
        return 1;
    }
  }
}
