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

int main() {
    vial_dets_t v1 = { 0 };
    struct AES_ctx ctx,ctx1,ctx2,ctx3;

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
    decrypt_vial(&ctx1, &v1);
    print_vial(&v1);

    //table stuff
    token_table_t table = { 0 };
    token_table_t table_1 = { 0 };
    set_table(&table);

    //need a buffer of 1088 bytes for an input of 816 bytes
    //TODO: add table encryption logic
    char encoded_table[1088] = { 0 };

    //try to use the same context
    encrypt_table(&ctx, &table, sizeof(table));
    encode_table(&table, encoded_table, sizeof(encoded_table));

    print_base64_chars(encoded_table, sizeof(encoded_table));

    decode_table(encoded_table, &table_1, sizeof(encoded_table), sizeof(table_1));
    decrypt_table(&ctx, &table_1, sizeof(table_1));

    printf("%X\n", table_1.rows[49].original_tokens);
    printf("%lX\n", table_1.table_version);

    //try the output of the machine
    char *input_machine_answer = "YlsEUJQ+gLVCHEwQkVuZtQ==";

    char *input_table = "2MVZPBDO7FzpmD6wnXjcH4MUAhO4FE/LBq52WM6EVOm4sotrqUsu6MOhClXJxpA/pcBl8CZwnArnB8IjCrmFF9jak6GARMjwipe70PY7TpVpbUTnKaXXT/QNwm9RrErFdReW8ySUj0St3uZTfw3lJd183ybKfx0qrLe7B7lJ6Uw1jX3AjL4NkN/HoJ2ry/IMT/DWiJfRH/ooYp+7NM9lNwn8k42FjMBJYrRjygGG0Z9rAV9PeBsbGCuUQQBpC42fVrTMrX+/xRGq8+5kQNPl374GY0+ddw1YFC56nNsn0+R/U4F9sFXcACob1jUM9kQNZCbK1vfP0a+9lEYI5L0D3BpHKK+2MsPtkqGYRu6/2gkour/YolAdfe1sHhdaMCpbzL0pCxutSjqquW9pi6YEzl0cT3Y4OtybxjoIWlhMd4DIjQP+xajP/9EqqwPssuDO3ZEiNTB/RgqoTkPVSqBYKB8IYYzv9r1gmjn/ME3mI7pv+omYBH0ep/xO2PuT68NpfHyQKcfcbg4R2o5TBw20d25WgV8LTO+hkjdykQ45ya7kQOyzomkkIhA/2Q3xVeYxLTlLJpPrhcsunmNv58CxRlAAvJ77/qwqFkGZDgjefkYmyPRWbUsiQNGoRWWUnN/4xd1LCqrqwB6ds4F/qg6cJn4LcPvdD0bI7GXTzLD/O1rUolKz6R2lT78dZn7MMMusY7qDWSygeDB/gtQ1WUc1KtyAnqqq3R+A4cZq6tsIRwJ/yNY9f8AoXKxd368FTTCB9XzmyGvvCm9cAEo+LFRjASOH2KT1z3qjoKYu5rfRVgDzrj81ERd4RAkz7F0blwEtUlwCPe5DxeqO1nNVkMTBGV9V7clCQpOA4DNZYO39v1Vcj6f+sTfZeyFjQAutRKAQTfZUXbgob7CXg/OEqqnwUhmhZGquuqCFeXy6TNzE1bXmTv/YsDucI57x5ZeeQj8wOpHLkxLjHz1uRxoknpS5RW3HT/iBT9VOdiG9b9M7BrEdaxkQ7FnlyTra0l5D/WDeBOVWkUvuziNUAd9N9Q0+2GazEcoSOUdMmK8BfNZuRQGAJnrrKU9GB9F+EZxZU6N8";

    //base64 decode the vial first
    vial_dets_t new_vial = { 0 };

    decode_vial(input_machine_answer, &new_vial, strlen(input_machine_answer));

    decrypt_vial(&ctx2, &new_vial);
    print_vial(&new_vial);

    //decode the table now 
    token_table_t new_table = { 0 };
    decode_table(input_table, &new_table, strlen(input_table), sizeof(new_table));

    //decrypt table
    decrypt_table(&ctx3, &new_table, sizeof(new_table));

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