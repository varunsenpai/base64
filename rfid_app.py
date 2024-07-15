import ctypes
import base64
import sys

so_file = "build/libbase64_app_lib.so"
my_functions = ctypes.CDLL(so_file)
my_functions.base64_encode.restype = ctypes.c_bool
my_functions.base64_encode.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_char), ctypes.c_uint16, ctypes.c_uint16]

class VialDets(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("magic_number", ctypes.c_uint8 * 3),
        ("version_number", ctypes.c_uint8),
        ("serial_number", ctypes.c_uint32),
        ("token_count", ctypes.c_uint32),
        ("original_token_count", ctypes.c_uint32)
    ]

def encode_vial_details(vial_details: VialDets, version_number = 0, serial_number = 0, token_count = 0, original_token_count = 0) -> str:

    vial_details.magic_number = (ctypes.c_uint8 * 3)(0x18, 0x56, 0xEB)
    vial_details.version_number = version_number
    vial_details.serial_number = serial_number
    vial_details.token_count = token_count
    vial_details.original_token_count = original_token_count

    # Convert vial_details to bytes to match the expected input format for base64_encode
    input_data = bytearray(vial_details.magic_number) + bytearray([vial_details.version_number]) + \
                 bytearray(vial_details.serial_number.to_bytes(4, byteorder='little', signed=False)) + \
                 bytearray(vial_details.token_count.to_bytes(4, byteorder='little', signed=False)) + \
                 bytearray(vial_details.original_token_count.to_bytes(4, byteorder='little', signed=False))
    
    input_size = len(input_data)
    input_buffer = (ctypes.c_uint8 * input_size)(*input_data)
    
    # Allocate memory for the output buffer (assuming a base64 encoded string size)
    output_size = 4 * ((input_size + 2) // 3)
    output_buffer = (ctypes.c_char * output_size)()
    
    # Call the base64_encode function
    success = my_functions.base64_encode(input_buffer, output_buffer, ctypes.c_uint16(input_size), ctypes.c_uint16(output_size))
    
    # Check if the function was successful and print the output
    if success:
        # Convert output buffer to a Python string
        output_string = ctypes.string_at(output_buffer, output_size).decode('ascii')
        return output_string
    else:
        print("Base64 encoding failed")

def decode_vial_details(encoded_string: str, vial: VialDets):
    input_hex = base64.b64decode(encoded_string)
    ctypes.memmove(ctypes.addressof(vial), input_hex, ctypes.sizeof(vial))

def print_vial(vial: VialDets):
    print("Magic Number:", ''.join(f"{x:02x}" for x in vial.magic_number))
    print("Version Number:", f"{vial.version_number:02x}")
    print("Serial Number:", f"{vial.serial_number:08x}")
    print("Token Count:", f"{vial.token_count:08x}")
    print("Original Token Count:", f"{vial.original_token_count:08x}")


def main():
    input_vial = VialDets()
    decoded_vial = VialDets()
    output_string = encode_vial_details(input_vial, 1, 0xABCDEF02, 0xFFFE, 0xFFFD)
    decode_vial_details(output_string, decoded_vial)
    print_vial(decoded_vial)

if __name__ == "__main__":
    main()