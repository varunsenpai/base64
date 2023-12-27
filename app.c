#define CBC 1

#include "base64.h"
#include "aes.h"
#include "srv_token.h"
#include <string.h>
#include <stdio.h>

char *iv = "ThisistheIV16byt";
char *key = "Thisisthekey16Bt";

static void print_array(uint8_t *array, uint8_t size);
static void print_vial(vial_dets_t *vial);
static void print_base64_chars(char *encoded_array, uint8_t size);
static void encrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static void decrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static bool encode_vial(vial_dets_t *vial, char *encoded_output, uint8_t output_size);

int main() {
    vial_dets_t v1 = { 0 };
    struct AES_ctx ctx;

    vial_init(&v1);
    vial_set_serial_num(&v1, 0xABCDEF02);
    vial_set_token_count(&v1, 0xFFFE);
    vial_set_original_token_count(&v1, 0xFFFD);

    //base64 encode the array
    char encoded_array[24] = { 0 };
    encrypt_vial(&ctx, &v1);
    encode_vial(&v1,encoded_array, sizeof(encoded_array));
    print_base64_chars(encoded_array, sizeof(encoded_array));
    print_vial(&v1);

    //decrypt
    base64_decode(encoded_array, (uint8_t *)&v1, sizeof(encoded_array), sizeof(v1));
    decrypt_vial(&ctx, &v1);
    print_vial(&v1);
}

static void print_array(uint8_t *array, uint8_t size){
    for(int i = 0; i < size; i++) {
        printf("%02X ", array[i]);
    }
    printf("\n");
}

static void print_vial(vial_dets_t *vial) {
    printf("The magic number is %02X%02X%02X\n", vial->magic_number[0], vial->magic_number[1], vial->magic_number[2]);
    printf("The version is %02X\n", vial->version_number);
    printf("The serial number is %X\n", vial->serial_number);
    printf("The token count is %X\n", vial->token_count);
    printf("The original_token_count is %X\n", vial->original_token_count);
}

static void print_base64_chars(char *encoded_array, uint8_t size) {
    for(int i = 0; i < size; i++) {
        printf("%c", encoded_array[i]);
    }
    printf("\n");
}

static void encrypt_vial(struct AES_ctx *ctx, vial_dets_t *v1) {
    AES_init_ctx_iv(ctx, key, iv);
    AES_CBC_encrypt_buffer(ctx, (uint8_t *)v1, sizeof(*v1));
}

static void decrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial) {
    AES_ctx_set_iv(ctx, iv);
    AES_CBC_decrypt_buffer(ctx, (uint8_t *)vial, sizeof(*vial));
}

static bool encode_vial(vial_dets_t *vial, char *encoded_output, uint8_t output_size) {
    bool status = base64_encode((uint8_t *)vial, encoded_output, sizeof(*vial), output_size);

    return status;
}