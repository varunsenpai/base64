#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
bool base64_encode(uint8_t *input, char *output, uint8_t input_size, uint8_t output_size);

int main() {
    //uint8_t input[] = {0x4D};
    uint8_t input[] = {0x18,0x56,0xEB,0x01,0xAB,0xCD,0xEF,0x02,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF};
    char output_buf[24] = { 0 };
    //char output_buf[4] = { 0 };

    base64_encode(input, output_buf, sizeof(input), sizeof(output_buf));
    printf("%s\n", output_buf);
}

bool base64_encode(uint8_t *input, char *output, uint8_t input_size, uint8_t output_size) {

    //check if output buff is enough for encoded base64 output
    int required_buff_size = 4 * ((int)(input_size/3) + (input_size % 3 != 0));
    printf("the required buff size is %d\n", required_buff_size);

    if(required_buff_size > output_size)
    {
        printf("output buff size is not sufficient\n");
        return false;
    }

    uint8_t count = 0;
    uint8_t state = 0;
    uint32_t process_three_bytes = 0;
    uint16_t output_buf_iterator = 0;
    uint8_t first_6_bits = 0;
    uint8_t second_6_bits = 0;
    uint8_t third_6_bits = 0;
    uint8_t fourth_6_bits = 0;
    uint8_t i = 0;
    
    bool stop_flow = false;
    while((i < input_size) && (!stop_flow))
    {        
        switch(state) {
            case 0:
                process_three_bytes |= (input[i] << 24);
                ++count;
                if((i + 1) < input_size)
                {
                    ++i;
                    process_three_bytes |= (input[i] << 16);
                    ++count;
                }
                if((i + 1) < input_size)
                {
                    ++i;
                    process_three_bytes |= (input[i] << 8);
                    count = 0;
                }
                ++state;
                break;
            //processed 3 bytes
            case 1:
                first_6_bits = (process_three_bytes & 0xFC000000) >> 26;
                second_6_bits = (process_three_bytes & 0x03F00000) >> 20;
                third_6_bits = (process_three_bytes & 0x000FC000) >> 14;
                fourth_6_bits = (process_three_bytes & 0x00003F00) >> 8;
                process_three_bytes = 0;
                ++state;
                break;
            //copy to output buf
            case 2:
                output[output_buf_iterator] = base64[first_6_bits];
                ++output_buf_iterator;
                output[output_buf_iterator] = base64[second_6_bits];
                ++output_buf_iterator;
                if(count == 1) {
                    output[output_buf_iterator] = '=';
                    ++output_buf_iterator;
                    output[output_buf_iterator] = '=';
                }
                else if(count == 2) {
                    output[output_buf_iterator] = base64[third_6_bits];
                    ++output_buf_iterator;
                    output[output_buf_iterator] = '=';
                }
                else {
                    output[output_buf_iterator] = base64[third_6_bits];
                    ++output_buf_iterator;
                    output[output_buf_iterator] = base64[fourth_6_bits];
                    ++output_buf_iterator;
                }

                first_6_bits = second_6_bits = third_6_bits = fourth_6_bits = 0;
                if(count) {
                    stop_flow = true;
                }
                else {
                    ++i;
                    state = 0;
                }
                break;
            default:
                printf("Noooooo1\n");
        }
    }

    return true;
}












    // three_bytes_combined = (input[0] << 24) | (input[1] << 16) | (input[2] << 8); 

    // //start extracting the 6 bits from msb 
    // uint8_t first_6_bits = (three_bytes_combined & 0xFC000000) >> 26;
    // uint8_t second_6_bits = (three_bytes_combined & 0x03F00000) >> 20;
    // uint8_t third_6_bits = (three_bytes_combined & 0x000FC000) >> 14;
    // uint8_t fourth_6_bits = (three_bytes_combined & 0x00003F00) >> 8;

    // //find this character in base64 character set
    // output[0] = base64[first_6_bits];
    // output[1] = base64[second_6_bits];
    // output[2] = base64[third_6_bits];
    // output[3] = base64[fourth_6_bits];