# kl26z HIC

Based on MKL26Z128VFM4 chip ([Data Sheet](https://www.nxp.com/docs/en/data-sheet/KL26P64M48SF5.pdf)):
- Cortex-M0+ 48 Mhz
- 128 KB Flash
- 16 KB RAM
- Full-speed USB 2.0 OTG controller: up to 16 bi-directional endpoints including EP0 (*)
- HVQFN32 packaging

(*) "16 bidirectional endpoints can be supported" (source: [KL26P121M48SF4RM :lock:](https://www.nxp.com/webapp/Download?colCode=KL26P121M48SF4RM))

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0000_0000 | 0x0002_0000 |
| SRAM     |  16 KB | 0x1FFF_F000 | 0x2000_3000 |

Bootloader size is 32 KB

## DAPLink default pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD         |
| SWCLK       |  O  | PTC5    |  26 |
| SWDIO       | I/O | PTC6    |  27 |
| SWO         |  I  | PTC7    |  28 |
| nRESET      |  O  | PTC8    |  22 |
| UART        |
| UART RX     |  I  | PTC3    |  24 |
| UART TX     |  O  | PTC4    |  25 |
| Button      |
| F-RST  But. |  I  | PTB1    |  21 |
| LEDs        |
| HID LED     |  O  | PTD4    |  30 |
| MSD LED     |  O  | PTD5    |  29 |
| CDC LED     |  O  | PTD6    |  31 |
