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
static void print_base64_chars(char *encoded_array, uint16_t size);
static void encrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static void decrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static bool encode_vial(vial_dets_t *vial, char *encoded_output, uint8_t output_size);
static bool encode_table(token_table_t *table, char *encoded_output, uint16_t output_size);
static bool decode_table(char *encoded_table, token_table_t *table, uint16_t input_size, uint16_t output_size);

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

    //table stuff
    token_table_t table = { 0 };
    token_table_t table_1 = { 0 };
    set_table(&table);

    //need a buffer of 1088 bytes for an input of 816 bytes
    char encoded_table[1088] = { 0 };
    encode_table(&table, encoded_table, sizeof(encoded_table));
    print_base64_chars(encoded_table, sizeof(encoded_table));
    decode_table(encoded_table, &table_1, sizeof(encoded_table), sizeof(table_1));

    printf("%X\n", table_1.rows[49].original_tokens);
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

static void print_base64_chars(char *encoded_array, uint16_t size) {
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

static bool encode_table(token_table_t *table, char *encoded_output, uint16_t output_size) {
    bool status = base64_encode((uint8_t *)table, encoded_output, sizeof(*table), output_size);

    return status;
}

static bool decode_table(char *encoded_table, token_table_t *table, uint16_t input_size, uint16_t output_size) {
    bool status = base64_decode(encoded_table, (uint8_t *)table, input_size, output_size);

    return status;
}