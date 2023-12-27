#ifndef __BASE_64__
#define __BASE_64__
#include <stdbool.h>
#include <stdint.h>

bool base64_encode(uint8_t *input, char *output, uint16_t input_size, uint16_t output_size);
bool base64_decode(char *input, uint8_t *output, uint16_t input_size, uint16_t output_size);

#endif