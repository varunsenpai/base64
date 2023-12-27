#ifndef __SRV_TOKEN_H__
#define __SRV_TOKEN_H__

#include <stdint.h>
#include <stdbool.h>
#define SRV_TOKEN_TABLE_MAX_ENTRIES     50

/* Exported types */
typedef struct 
{
    uint32_t serial_number;                     /* Serial number of the card*/
    uint32_t tokens_remaining;                  /* Tokens left */
    uint32_t tokens_on_vial;                    /* Tokens on the vial */
    uint32_t original_tokens;                   /* Maximum capacity of tokens */
} rows_t;

//total size is 816 bytes
typedef struct 
{
    uint64_t table_version;                     /* Version to track changes */
    uint64_t padding_bytes;                     /* Pad the table to make it a multiple of 16 for AES encryption*/
    rows_t rows[SRV_TOKEN_TABLE_MAX_ENTRIES];   /* Card details, MAX of 50 for now */
} __attribute__((packed)) token_table_t;

typedef struct
{
    uint8_t magic_number[3];
    uint8_t version_number;
    uint32_t serial_number;
    uint32_t token_count;
    uint32_t original_token_count;
} __attribute__((packed)) vial_dets_t;

bool vial_init(vial_dets_t *vial);
bool vial_set_serial_num(vial_dets_t *vial, uint32_t serial_number);
bool vial_set_token_count(vial_dets_t *vial, uint32_t token_count);
bool vial_set_original_token_count(vial_dets_t *vial, uint32_t original_token_count);

void set_table(token_table_t *table);

#endif /* __SRV_TOKEN_H__ */