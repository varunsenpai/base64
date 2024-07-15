#define CBC 1

#include "base64.h"
#include "aes.h"
#include "srv_token.h"
#include <string.h>
#include <stdio.h>

const uint8_t iv[] = {0x54,0x68,0x69,0x73,0x69,0x73,0x74,0x68,0x65,0x49,0x56,0x31,0x36,0x62,0x79,0x74};
char *key = "Thisisthekey16BtThisisthekey16Bt";

static void print_array(uint8_t *array, uint8_t size);
static void print_vial(vial_dets_t *vial);
static void print_base64_chars(char *encoded_array, uint16_t size);
static void encrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static void decrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial);
static bool encode_vial(vial_dets_t *vial, char *encoded_output, uint8_t output_size);
static bool decode_vial(char *encoded_input, vial_dets_t *vial, uint8_t input_size);
static bool encode_table(token_table_t *table, char *encoded_output, uint16_t output_size);
static bool decode_table(char *encoded_table, token_table_t *table, uint16_t input_size, uint16_t output_size);
static void encrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size);
static void decrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size);
static void print_string_array(char *array, uint8_t size);

int main() {
    vial_dets_t v1 = { 0 };
    struct AES_ctx ctx,ctx1,ctx2,ctx3,ctx4;

    vial_init(&v1);
    vial_set_serial_num(&v1, 0xABCDEF02);
    vial_set_token_count(&v1, 0xFFFE);
    vial_set_original_token_count(&v1, 0xFFFD);

    //base64 encode the array
    char encoded_array[24] = { 0 };
    encrypt_vial(&ctx, &v1);
    encode_vial(&v1,encoded_array, sizeof(encoded_array));
    print_base64_chars(encoded_array, sizeof(encoded_array));
    //print_vial(&v1);

    //decrypt
    base64_decode(encoded_array, (uint8_t *)&v1, sizeof(encoded_array), sizeof(v1));
    decrypt_vial(&ctx1, &v1);
    //print_vial(&v1);

    // //table stuff
    // token_table_t table = { 0 };
    // token_table_t table_1 = { 0 };
    // set_table(&table);

    // //need a buffer of 1088 bytes for an input of 816 bytes
    // //TODO: add table encryption logic
    // char encoded_table[1088] = { 0 };

    // //try to use the same context
    // encrypt_table(&ctx, &table, sizeof(table));
    // encode_table(&table, encoded_table, sizeof(encoded_table));

    // print_base64_chars(encoded_table, sizeof(encoded_table));

    // decode_table(encoded_table, &table_1, sizeof(encoded_table), sizeof(table_1));
    // decrypt_table(&ctx, &table_1, sizeof(table_1));

    //printf("%X\n", table_1.rows[49].original_tokens);
    //printf("%lX\n", table_1.table_version);

    //try the output of the machine
    char *input_machine_answer = "YlsEUJQ+gLVCHEwQkVuZtQ==";

    char *input_table = "qMyJrasqbOSEehxU4Dbrq7NJIIy6isG0zo0p+hw99TFWxlVBFoR4sgy9Bwt8BarEz0S/GwEIEl1CJaPmOCG8L/v0NjGXAzYoLPkylFvufnLDfRd10gRBl84T595xsktC3mARiwLWF6Q+7xpCxZCWtHfOyDdZzqhpeZRsAEzGOyQgH8Pza55T0YaFCWpSDDdNaYc95n3nThPKkd9WjPE6NNfAYgczWVwoI59BOwGrCrvc4goYW1WZY1qpjx3KxHkbuvkKjFhhV3GKi1QzThZzna2/2OAYAmQjbG43GLOMeMTPDDUXcCvXKS+c9axcgBdNvrXmf2GGoLXE/fAuBfW0JWBgjhl+m/Plvga5hQ1sAQ98rQ3mNfP3c/rxVJi53h062bYbl+/VehuSkaVXJwzIrD0uam2leqc7eBEMU2OkK1AZbIY/PaTmwWO3RpAadxju6mm3JH6BRbdyS7kGaz2cxLzMPnZddQiWwy5tInzX8YqfNMcTs/8yy443H9RbX2yuzLyOeAF9qdnnD+myeaTZCr9U1zxjixIU3CCVt8A5Kar1uuZLHaLEXFUV1o65k1QSUAVprHhtNuk03HaAa+fj4u8veVOuoJx2uvUQ+EaiyXjjK1/Hrfk2m08EBKvb4gZwUtlWQyZTpKdedAwKXHoRT1ua2JoTWDoUjfSJrTgLklYZpFV8Du57yH+XW59fB8DmS9Gx0o/tuMsrd/TlfnITe3QnmG1FR3Q2SXVR86GyGfmeL1aOXWpmQP8tkj/fJB9QkZBozpRshn10v1ZAsdDQICbBkgf33aTBiKCm2yDJw5HBbBzxTmR1OW9d8G0vxLJTlyLIc4Ngm4Lhws4q3YM4a8wahvIhjJCjgk/0gDtO44hlJdNZvAizIr4yguP2w2FB/Ai9Okls7xDUMCK8PG7aNUiweq5z4Gft1+JZa1a7Xp86UW1bycEJNMWrZBi/e6EBVFygn1QEMPyOQ1TpcZmHU+zl7LZjeQwcDofLxvxNB2znRjW0qcbbYdC1hPuZqGeOv0SIgJUGfv3eA4FN1+NLwI6tph2f030o3EsJFhgVtU3zX458+e5gNx+LW7fJKl5t";

    //base64 decode the vial first
    // vial_dets_t new_vial = { 0 };

    // decode_vial(input_machine_answer, &new_vial, strlen(input_machine_answer));

    // decrypt_vial(&ctx2, &new_vial);
    //print_vial(&new_vial);

    //decode the table now 
    token_table_t new_table = { 0 };
    decode_table(input_table, &new_table, strlen(input_table), sizeof(new_table));

    //decrypt table
    decrypt_table(&ctx3, &new_table, sizeof(new_table));

    //print fields in the table 
    printf("the version is %ld\n", new_table.table_version);
    printf("the padding bytes are %ld\n", new_table.padding_bytes);
    printf("the serial number is %02X\n", new_table.rows[0].serial_number);
    printf("the tokens remaining are %X\n", new_table.rows[0].tokens_remaining);
    printf("the tokens on vial are %X\n", new_table.rows[0].tokens_on_vial);
    printf("the original tokens are %X\n", new_table.rows[0].tokens_on_vial);

    //to test the set table logic now
    // iterate every 24 base64 characters, which is 16 bytes
    uint8_t table_to_split[816] = { 0 };
    memcpy(table_to_split, &new_table, sizeof(table_to_split));

    char temp[24] = { 0 };
    AES_ctx_set_iv(&ctx4,iv);
    AES_init_ctx_iv(&ctx4, key, iv);
    for(uint8_t i = 0; i < 51; ++i) {
        AES_CBC_encrypt_buffer(&ctx4, &(table_to_split[16 * i]), 16);
        base64_encode(&(table_to_split[16 * i]), temp, 16, 24);
        print_string_array(temp, 24);
        memset(temp, 0, sizeof(temp));    
    }


}

static void print_string_array(char *array, uint8_t size) {
    for(int i = 0; i < size; i++) {
        printf("%c", array[i]);
    }
    printf("\n");
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
    AES_init_ctx_iv(ctx, (uint8_t *)key, iv);
    AES_ctx_set_iv(ctx, iv);
    AES_CBC_decrypt_buffer(ctx, (uint8_t *)vial, sizeof(*vial));
}

static bool encode_vial(vial_dets_t *vial, char *encoded_output, uint8_t output_size) {
    bool status = base64_encode((uint8_t *)vial, encoded_output, sizeof(*vial), output_size);

    return status;
}

static bool decode_vial(char *encoded_input, vial_dets_t *vial, uint8_t input_size) {
    bool status = base64_decode(encoded_input, (uint8_t *)vial, input_size, sizeof(*vial));

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

static void encrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size) {
    AES_ctx_set_iv(ctx,iv);
    AES_init_ctx_iv(ctx, key, iv);
    AES_CBC_encrypt_buffer(ctx, (uint8_t *)table, size);
}

static void decrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size) {
    AES_init_ctx_iv(ctx, (uint8_t *)key, iv);
    AES_ctx_set_iv(ctx,iv);
    AES_CBC_decrypt_buffer(ctx, (uint8_t *)table, size);
}