#ifndef __BASE_64__
#define __BASE_64__
#include <stdbool.h>
#include <stdint.h>

bool base64_encode(uint8_t *input, char *output, uint8_t input_size, uint8_t output_size);
bool base64_decode(char *input, uint8_t *output, uint8_t input_size, uint8_t output_size);

#endif