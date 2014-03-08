#!/bin/bash

mkxxx_dir="$(pwd)"
tools_dir="$(pwd)/../../../tools"
tmp_dir="$tools_dir/tmp"

flash_algo_axf="$tmp_dir/flash_algo.axf"
flash_algo_txt="$tmp_dir/flash_algo.txt"

targets="MK20DX128 MK64FN1M0 MKL02Z32 MKL05Z32 MKL25Z128 MKL26Z128 MKL46Z256"

# Make tmp dir if it doesn't exist.
if [ ! -d "$tmp_dir" ]; then
    mkdir "$tmp_dir"
fi

# Process each target axf.
for targ in $targets; do
    target_axf="./Output/$targ/$targ.axf"
    target_txt="./$targ.txt"
    
    rm -f "$tmp_dir"/*
    cp "$target_axf" "$flash_algo_axf"
    
    pushd "$tools_dir"
    python flash_algo_gen.py
    popd
    
    mv "$flash_algo_txt" "$target_txt"
done

