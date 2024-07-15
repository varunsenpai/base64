import ctypes
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
        ("original_topken_count", ctypes.c_uint32)
    ]

def encode_vial_details(vial_details: VialDets):

    vial_details.magic_number = (ctypes.c_uint8 * 3)(0x18, 0x56, 0xEB)
    vial_details.version_number = 1
    vial_details.serial_number = 0xABCDEF02
    vial_details.token_count = 0xFFFE
    vial_details.original_token_count = 0xFFFD

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
        print("Base64 Encoded Output:", output_string)
    else:
        print("Base64 encoding failed")

def main():
    input_vial = VialDets()
    encode_vial_details(input_vial)

if __name__ == "__main__":
    main()