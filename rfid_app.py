import ctypes
import base64
import sys

class AES_ctx(ctypes.Structure):
    _fields_ = [
        ("RoundKey", ctypes.c_uint8 * 240),
        ("Iv", ctypes.c_uint8 * 16)
    ]

class VialDets(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("magic_number", ctypes.c_uint8 * 3),
        ("version_number", ctypes.c_uint8),
        ("serial_number", ctypes.c_uint32),
        ("token_count", ctypes.c_uint32),
        ("original_token_count", ctypes.c_uint32)
    ]

# Define the maximum number of entries
SRV_TOKEN_TABLE_MAX_ENTRIES = 50

# Define the rows_t structure
class Rows(ctypes.Structure):
    _fields_ = [
        ('serial_number', ctypes.c_uint32),             # Serial number of the card
        ('tokens_remaining', ctypes.c_uint32),          # Tokens left
        ('tokens_on_vial', ctypes.c_uint32),            # Tokens on the vial
        ('original_tokens', ctypes.c_uint32),           # Maximum capacity of tokens
    ]

# Define the token_table_t structure
class TokenTable(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ('table_version', ctypes.c_uint64),              # Version to track changes
        ('padding_bytes', ctypes.c_uint64),              # Pad the table to make it a multiple of 16 for AES encryption
        ('rows', Rows * SRV_TOKEN_TABLE_MAX_ENTRIES),    # Card details, MAX of 50 for now
    ]

so_file = "build/libbase64_app_lib.so"
my_functions = ctypes.CDLL(so_file)
my_functions.base64_encode.restype = ctypes.c_bool
my_functions.base64_encode.argtypes = [ctypes.POINTER(ctypes.c_uint8), ctypes.POINTER(ctypes.c_char), ctypes.c_uint16, ctypes.c_uint16]
my_functions.encrypt_vial.argtypes = [ctypes.POINTER(AES_ctx), ctypes.POINTER(VialDets)]
my_functions.decrypt_vial.argtypes = [ctypes.POINTER(AES_ctx), ctypes.POINTER(VialDets)]
my_functions.encrypt_table.argtypes = [ctypes.POINTER(AES_ctx), ctypes.POINTER(TokenTable), ctypes.c_uint16]
my_functions.decrypt_table.argtypes = [ctypes.POINTER(AES_ctx), ctypes.POINTER(TokenTable), ctypes.c_uint16]
my_functions.encode_table.argtypes = [ctypes.POINTER(TokenTable), ctypes.c_char_p, ctypes.c_uint16]
my_functions.encode_table.restype = ctypes.c_bool


def populate_vial(vial_details: VialDets, version_number = 0, serial_number = 0, token_count = 0, original_token_count = 0):
    vial_details.magic_number = (ctypes.c_uint8 * 3)(0x18, 0x56, 0xEB)
    vial_details.version_number = version_number
    vial_details.serial_number = serial_number
    vial_details.token_count = token_count
    vial_details.original_token_count = original_token_count    

def encode_vial_details(vial_details: VialDets) -> str:

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

def encrypt_vial(vial: VialDets):
    aes_ctx = AES_ctx()
    my_functions.encrypt_vial(ctypes.byref(aes_ctx), ctypes.byref(vial))

def decrypt_vial(vial: VialDets):
    aes_ctx = AES_ctx()
    my_functions.decrypt_vial(ctypes.byref(aes_ctx), ctypes.byref(vial))

def encrypt_table(table: TokenTable):
    aes_ctx = AES_ctx()
    my_functions.encrypt_table(ctypes.byref(aes_ctx), ctypes.byref(table), ctypes.sizeof(table))

def decrypt_table(table: TokenTable):
    aes_ctx = AES_ctx()
    my_functions.decrypt_table(ctypes.byref(aes_ctx), ctypes.byref(table), ctypes.sizeof(table))

def encode_table(table: TokenTable):
    encoded_output = ctypes.create_string_buffer(1088)
    result = my_functions.encode_table(ctypes.byref(table), encoded_output, 1088)

    if result:
        return encoded_output
    else:
        print('table encode failed')

def decode_table(table:TokenTable, input_encoded_table):
    result = my_functions.decode_table(input_encoded_table, ctypes.byref(table), ctypes.sizeof(table), 1088)

    if not result:
        print('table encode failed')

def print_vial(vial: VialDets):
    print("Magic Number:", ''.join(f"{x:02x}" for x in vial.magic_number))
    print("Version Number:", f"{vial.version_number:02x}")
    print("Serial Number:", f"{vial.serial_number:08x}")
    print("Token Count:", f"{vial.token_count:08x}")
    print("Original Token Count:", f"{vial.original_token_count:08x}")


def main():
    input_vial = VialDets()
    decoded_vial = VialDets()
    populate_vial(input_vial, 1, 0xABCDEF02, 0xFFFE, 0xFFFD)
    encrypt_vial(input_vial)
    output_string = encode_vial_details(input_vial)
    print(output_string)

    decode_vial_details(output_string, decoded_vial)
    decrypt_vial(decoded_vial)
    print_vial(decoded_vial)

    input_table = TokenTable()
    input_table.version = 1
    input_table.rows[0].serial_number = 0xABCDEF02
    input_table.rows[0].tokens_remaining = 100
    input_table.rows[0].tokens_on_vial = 50
    input_table.rows[0].original_tokens = 150

    encrypt_table(input_table)
    encoded_table = encode_table(input_table)
    print(encode_table)

    decoded_table = TokenTable()
    decode_table(decoded_table, encoded_table)
    decrypt_table(decoded_table)

    print(f'{decoded_table.rows[0].serial_number:08x}')
    print(f'{decoded_table.rows[0].tokens_remaining:08x}')
    print(f'{decoded_table.rows[0].tokens_on_vial:08x}')
    print(f'{decoded_table.rows[0].original_tokens:08x}')

if __name__ == "__main__":
    main()