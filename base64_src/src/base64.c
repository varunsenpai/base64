#include "base64.h"
#include <stdio.h>
#include <string.h>

const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static bool base64_char_position(char input_char, uint16_t *position);

bool base64_encode(uint8_t *input, char *output, uint16_t input_size, uint16_t output_size) {

    //check if output buff is enough for encoded base64 output
    int required_buff_size = 4 * ((int)(input_size/3) + (input_size % 3 != 0));

    if(required_buff_size > output_size)
    {
        printf("output buff size is not sufficient\n");
        return false;
    }

    uint16_t count = 0;
    uint8_t state = 0;
    uint32_t process_three_bytes = 0;
    uint16_t output_buf_iterator = 0;
    uint8_t first_6_bits = 0;
    uint8_t second_6_bits = 0;
    uint8_t third_6_bits = 0;
    uint8_t fourth_6_bits = 0;
    uint16_t i = 0;
    
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

bool base64_decode(char *input, uint8_t *output, uint16_t input_size, uint16_t output_size) {

    //check if the input is a multiple of 4
    if(input_size % 4) {
        printf("not a base64 encoded string");
        return false;
    }

    //count no of padding characters
    uint16_t pad_chars = 0;

    for(int i = 0; i < input_size; i++)
    {
        if(input[i] == '=')
            ++pad_chars;
    }

    //check required buf size for decoded output
    uint16_t req_buf_size = 3 * (uint16_t)(input_size / 4) - pad_chars;

    if(req_buf_size > output_size)
    {
        printf("output buffer size not big enough\n");
        return false;
    }

    uint16_t i = 0;
    bool stop_flow = false;
    uint8_t state = 0;
    uint32_t process_4_chars = 0;
    uint16_t val1 = 0;
    uint16_t val2 = 0;
    uint16_t val3 = 0;
    uint16_t val4 = 0;
    uint16_t output_buf_iterator = 0;

    //group 4 base64 characters and split them into 3 bytes separately
    while(i <(input_size) && !stop_flow)
    {
        switch(state)
        {
            //find the character and store  
            case 0:
                //process 4 chars
                if(input[i] != '=')
                {
                    base64_char_position(input[i], &val1);    
                }
                else
                {
                    stop_flow = true;
                }

                if(((i + 1) < input_size) && input[i + 1] != '=')
                {
                    ++i;
                    base64_char_position(input[i], &val2);
                }
                else
                {
                    stop_flow = true;
                }
                if(((i + 1) < input_size) && input[i + 1] != '=')
                {
                    ++i;
                    base64_char_position(input[i], &val3);
                }
                else
                {
                    stop_flow = true;
                }                
                if(((i + 1) < input_size) && input[i + 1] != '=')
                {
                    ++i;
                    base64_char_position(input[i], &val4);
                    ++i;
                }
                else
                {
                    stop_flow = true;
                }

                process_4_chars |= val1 << 26;
                process_4_chars |= val2 << 20;
                process_4_chars |= val3 << 14;
                process_4_chars |= val4 << 8;
                
                if(output_buf_iterator < output_size) {
                    output[output_buf_iterator] = (process_4_chars & 0xFF000000) >> 24;
                    ++output_buf_iterator;
                }
                if(output_buf_iterator < output_size) {
                    output[output_buf_iterator] = (process_4_chars & 0x00FF0000) >> 16;
                    ++output_buf_iterator;
                }
                if(output_buf_iterator < output_size) {
                    output[output_buf_iterator] = (process_4_chars & 0x0000FF00) >> 8;
                    ++output_buf_iterator;
                }
                process_4_chars = 0;
                val1 = val2 = val3 = val4 = 0;
                ++state;
                break;
            case 1:
                if(!stop_flow)
                    state = 0;
                break;
            default: 
                printf("default\n"); 
        }
    }
}

static bool base64_char_position(char input_char, uint16_t *position) {
    bool found = false;

    for(int i = 0; i < sizeof(base64); i++)
    {
        if(input_char == base64[i]) {
            *position = i;
            found = true;
            break;
        }
    }

    if(!found) {
        printf("could not find char in base64 char map\n");
    }

    return found;
}