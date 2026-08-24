#!/usr/bin/env bash

pip3 install --user -r requirements.txt
sudo apt install -y ccache ninja-build

python3 tools/progen_compile.py --release --parallel -v
