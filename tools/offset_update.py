import argparse


def dec_or_hex(val):
    return int(val, 0)


def main():
    parser = argparse.ArgumentParser(description='File Padder')
    parser.add_argument("bin", type=str, default=None, help="Input binary file")
    parser.add_argument("--start", type=dec_or_hex, default=0x8000,
                        help="Starting address of input binary file")
    parser.add_argument("--padded_start", type=dec_or_hex, default=0x5000,
                        help="Starting address after padding.")
    parser.add_argument("--output", type=str, required=True,
                        help="Output file")
    parser.add_argument("--copysize", type=str, default=0x40,
                        help="Size of original binary to copy")
    args = parser.parse_args()

    # Data is appened to front so padded start must be less than start
    assert args.start > args.padded_start

    pad_size = args.start - args.padded_start
    copy_size = args.copysize
    input_file = args.bin
    output_file = args.output
    with open(input_file, 'rb') as file_handle:
        data = file_handle.read()

    output_data = data[0:copy_size] + '\xff' * (pad_size - copy_size) + data
    with open(output_file, 'wb') as file_handle:
        data = file_handle.write(output_data)


if __name__ == "__main__":
    main()
