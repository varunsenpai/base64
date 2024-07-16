#include "srv_token.h"

const uint8_t iv[] = {0x54,0x68,0x69,0x73,0x69,0x73,0x74,0x68,0x65,0x49,0x56,0x31,0x36,0x62,0x79,0x74};
char *key = "Thisisthekey16BtThisisthekey16Bt";

bool vial_init(vial_dets_t *vial) {
    vial->magic_number[0] = 0x18;
    vial->magic_number[1] = 0x56;
    vial->magic_number[2] = 0xEB;
    vial->version_number = 1;
}

bool vial_set_serial_num(vial_dets_t *vial, uint32_t serial_number) {
    vial->serial_number = serial_number;
}

bool vial_set_token_count(vial_dets_t *vial, uint32_t token_count) {
    vial->token_count = token_count;
}

bool vial_set_original_token_count(vial_dets_t *vial, uint32_t original_token_count) {
    vial->original_token_count = original_token_count;
}

void encrypt_vial(struct AES_ctx *ctx, vial_dets_t *v1) {
    AES_init_ctx_iv(ctx, key, iv);
    AES_CBC_encrypt_buffer(ctx, (uint8_t *)v1, sizeof(*v1));
}

void decrypt_vial(struct AES_ctx *ctx, vial_dets_t *vial) {
    AES_init_ctx_iv(ctx, (uint8_t *)key, iv);
    AES_ctx_set_iv(ctx, iv);
    AES_CBC_decrypt_buffer(ctx, (uint8_t *)vial, sizeof(*vial));
}
void set_table(token_table_t *table) {
    table->table_version = 0x01;
    table->padding_bytes = 0x00;
    
    for(int i = 0; i < SRV_TOKEN_TABLE_MAX_ENTRIES; i++) {
        table->rows[i].serial_number = 0xABCDEF01;
        table->rows[i].tokens_on_vial = 0xFF;
        table->rows[i].tokens_remaining = 0xFF;
        table->rows[i].original_tokens = 0xFF;
    }
}

bool encode_table(token_table_t *table, char *encoded_output, uint16_t output_size) {
    bool status = base64_encode((uint8_t *)table, encoded_output, sizeof(*table), output_size);

    return status;
}

bool decode_table(char *encoded_table, token_table_t *table, uint16_t input_size, uint16_t output_size) {
    bool status = base64_decode(encoded_table, (uint8_t *)table, input_size, output_size);

    return status;
}

void encrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size) {
    AES_ctx_set_iv(ctx,iv);
    AES_init_ctx_iv(ctx, key, iv);
    AES_CBC_encrypt_buffer(ctx, (uint8_t *)table, size);
}

void decrypt_table(struct AES_ctx *ctx, token_table_t *table, uint16_t size) {
    AES_init_ctx_iv(ctx, (uint8_t *)key, iv);
    AES_ctx_set_iv(ctx,iv);
    AES_CBC_decrypt_buffer(ctx, (uint8_t *)table, size);
}