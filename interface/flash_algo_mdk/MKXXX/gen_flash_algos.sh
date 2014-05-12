#!/bin/bash

# This script runs each of the flash algos through the flash_algo_gen.py script. It copies
# the output flash blob text file into the MKXXX directory and renames to to the target name.
#
# The script must be run from the MKXXX directory.

# Directories.
mkxxx_dir="$(pwd)"
tools_dir="$(pwd)/../../../tools"
tmp_dir="$tools_dir/tmp"

# File paths.
flash_algo_axf="$tmp_dir/flash_algo.axf"
flash_algo_txt="$tmp_dir/flash_algo.txt"

# List of targets to process.
targets="MK20DX128 \
        MK64FN1M0 \
        MKL02Z32 \
        MKL05Z32 \
        MKL25Z128 \
        MKL26Z128 \
        MKL46Z256"

# Make tmp dir if it doesn't exist.
if [ ! -d "$tmp_dir" ]; then
    mkdir "$tmp_dir"
fi

# Process each target axf.
for targ in $targets; do
    target_axf="./Output/$targ/$targ.axf"
    target_txt="./$targ.txt"
    
    # Remove existing target flash blob output file.
    if [ -f "$target_txt" ]; then
        rm -f "$target_txt"
    fi
    
    # Clean tmp dir and copy axf.
    rm -f "$tmp_dir"/*
    cp "$target_axf" "$flash_algo_axf"
    
    # Run the gen script.
    echo "Processing $targ..."
    pushd "$tools_dir" > /dev/null
    python flash_algo_gen.py
    popd > /dev/null
    
    # Copy output flash blob.
    mv "$flash_algo_txt" "$target_txt"
done

