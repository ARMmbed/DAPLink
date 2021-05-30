# k20dx HIC

Based on MK20DX128VFM5 chip ([Data Sheet](https://www.nxp.com/docs/en/data-sheet/K20P32M50SF0.pdf)):
- Cortex-M4 50 Mhz
- 160 KB Flash
- 16 KB RAM
- Full-speed USB 2.0 OTG controller
- HVQFN32 packaging

## Memory Map

| Region   |   Size | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0000_0000 | 0x0002_0000 |
| SRAM     |  16 KB | 0x1FFF_E000 | 0x2000_0000 |

Bootloader size is 32 KB

## DAPLink default pin assignment 

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD         |
| SWCLK       |  O  | PTC5    |  26 |
| SWDIO       |  O  | PTC6    |  27 |
| SWDIO       |  I  | PTC7    |  27 |
| nRESET      |  O  | PTB1    |  21 |
| UART        |
| UART RX     |  I  | PTC3    |  24 |
| UART TX     |  O  | PTC4    |  25 |
| Button      |
| NF-RST But. |  I  | PTB1    |  21 |
| LEDs        |
| LED         |  O  | PTD4    |  29 |
