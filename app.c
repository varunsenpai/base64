#include "base64.h"
#include <string.h>
#include <stdio.h>

int main() {
    char output_buf[12] = { 0 };
    char *input_1 = "GFbrAavN7w==";
    uint8_t output_buf_1[7] = { 0 };
    char *input_2 = "GFY=";
    uint8_t single_buf[2] = { 0 };

    base64_decode(input_1, output_buf_1, strlen(input_1), sizeof(output_buf_1));
    printf("%02X%02X%02X%02X%02X%02X%02X\n", output_buf_1[0],output_buf_1[1],output_buf_1[2], output_buf_1[3],output_buf_1[4],output_buf_1[5],output_buf_1[6]);
    base64_encode(output_buf_1, output_buf, sizeof(output_buf_1), sizeof(output_buf));
    printf("%s\n", output_buf);

    base64_decode(input_2, single_buf, strlen(input_2), sizeof(single_buf));
    printf("%02X%02X\n", single_buf[0], single_buf[1]);
}
