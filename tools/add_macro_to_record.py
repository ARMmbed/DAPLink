#!/usr/bin/env python
# This script can be used in build systems to add macros dynamically

from jsonschema import FormatError
import yaml
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("path_to_record_file")
    parser.add_argument("macro")
    args = parser.parse_args()

    file_name = args.path_to_record_file
    new_macro = args.macro

    with open(file_name, "r") as stream:
        record = yaml.safe_load(stream)

    if ("common" in record):
        if (("macros" in record["common"]) and (record["common"]["macros"]is not None)):
            record["common"]["macros"].append(new_macro)
        else:
            record["common"]["macros"] = [new_macro]
    else:
        raise FormatError("The record yaml file has the wrong format, are you sure the file path is correct?")


    with open(file_name, "w") as stream:
        yaml.dump(record, stream)
