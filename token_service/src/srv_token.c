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