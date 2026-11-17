python3 -m venv venv
source venv/bin/activate
python tools/progen_compile.py -t make_gcc_arm --clean --parallel stm32f411xe_bl stm32f411xe_if && pyocd flash -t stm32f411ceux   projectfiles/make_gcc_arm/stm32f411xe_bl/build/stm32f411xe_bl_crc.hex   projectfiles/make_gcc_arm/stm32f411xe_if/build/stm32f411xe_if_crc.hex

