#include "srv_token.h"

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