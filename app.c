#define CBC 1

#include "base64.h"
#include "aes.h"
#include <string.h>
#include <stdio.h>

char *iv = "ThisistheIV16byt";
char *key = "Thisisthekey16Bt";

static void print_array(uint8_t *array, uint8_t size);

int main() {
    uint8_t sample_array[16] = { 0 };
    struct AES_ctx ctx;

    for(int i = 0; i < sizeof(sample_array); i++) {
        sample_array[i] = i;
    }

    //base64 encode the array
    uint8_t encoded_array[24] = { 0 };
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, sample_array, sizeof(sample_array));
    print_array(sample_array, sizeof(sample_array));
    base64_encode(sample_array, encoded_array, sizeof(sample_array), sizeof(encoded_array));
    print_array(encoded_array, sizeof(encoded_array));

    //decrypt
    base64_decode(encoded_array, sample_array, sizeof(encoded_array), sizeof(sample_array));
    AES_ctx_set_iv(&ctx, iv);
    AES_CBC_decrypt_buffer(&ctx, sample_array, sizeof(sample_array));
    print_array(sample_array, sizeof(sample_array));
}

static void print_array(uint8_t *array, uint8_t size){
    for(int i = 0; i < size; i++) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}
