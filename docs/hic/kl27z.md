# kl27z HIC

Based on MKL27Z256VFM4 chip ([Data Sheet](https://www.nxp.com/docs/en/data-sheet/KL27P64M48SF6.pdf)):
- Cortex-M0+ 48 Mhz
- 256 KB Flash
- 32 KB RAM
- Full-speed USB 2.0 device controller: up to 16 bi-directional endpoints including EP0 (*)
- HVQFN32 packaging

(*) "16 bidirectional endpoints can be supported" (source: [KL27P64M48SF6RM :lock:](https://www.nxp.com/webapp/Download?colCode=KL27P64M48SF6RM))

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0000_0000 | 0x0002_0000 |
| SRAM     |  32 KB | 0x1FFF_E000 | 0x2000_6000 |

Bootloader size is 32 KB

## DAPLink default pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD         |
| SWCLK       |  O  | PTC6    |  27 |
| SWDIO       | I/O | PTC5    |  26 |
| nRESET      |  O  | PTA20   |  19 |
| UART        |
| UART RX     |  I  | PTA18   |  17 |
| UART TX     |  O  | PTA19   |  18 |
| Button      |
| F-RST  But. |  I  | PTD4    |  29 |
| LEDs        |
| HID LED     |  O  | PTB0    |  20 |
| MSD LED     |  O  | PTD0    |  20 |
| CDC LED     |  O  | PTD0    |  20 |
