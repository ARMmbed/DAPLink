# sam3u2c HIC

Based on ATSAM3U2C chip ([Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-6430-32-bit-Cortex-M3-Microcontroller-SAM3U4-SAM3U2-SAM3U1_Datasheet.pdf)):
- Cortex-M3 96 Mhz
- 128 KB Flash
- 36 KB RAM
- High-speed USB 2.0 device controller: up to 7 bi-directional endpoints including EP0 (*)
- LQFP100 packaging

(*) "up to 7 bidirectional Endpoints" (source: Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-6430-32-bit-Cortex-M3-Microcontroller-SAM3U4-SAM3U2-SAM3U1_Datasheet.pdf))

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0000_0000 | 0x0001_0000 |
| SRAM     |  32 KB | 0x2007_C000 | 0x2008_3FFF |

Bootloader size is 32 KB

## DAPLink default pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD         |
| SWCLK       |  O  | PA17    |  14 |
| SWDIO       | I/O | PA18    |  17 |
| nRESET      |  O  | PA30    |  30 |
| UART        |
| UART TX     |  O  | PA12    |  41 |
| UART RX     |  I  | PA11    |  40 |
| Button      |
| NF-RST But. |  I  | PA25    |  24 |
| LEDs        |
| HID LED     |  O  | PA29    |  85 |
| MSD LED     |  O  | PA28    |  84 |
| CDC LED     |  O  | PA31    |  86 |

# J-Link OB SAM3U128

## Warning

The SEGGER design suggests a change to the pin assignment of the TC2050-IDC
used to debug the HIC MCU. They suggest replacing GNDDetect by power-supply
(VBus, V5), which could create a short with standard Tag-Connect cables and
SWD probes.

## J-Link OB SAM3U128 pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD / JTAG  |
| SWCLK / TCK |  O  | PA17    |  14 |
| SWCLK / TCK |  I  | PA22    |   5 |
| SWDIO / TMS |  O  | PA18    |  17 |
| SWDIO / TMS |  I  | PA15    |  12 |
| TDI         |  O  | PA20    |  19 |
| TDI         |  I  | PA13    |  10 |
| SWO / TDO   |  I  | PA21    |  20 |
| nRESET      |  O  | PA4     |  30 |
| nRESET      |  I  | PA3     |  29 |
| UART        |
| UART TX     |  O  | PA12    |  41 |
| UART RX     |  I  | PA11    |  40 |
| Button      |
| NF-RST But. |  O  | PA1     |  27 |
| NF-RST But. |  I  | PA0     |  26 |
| LEDs        |
| LED1        |  O  | PA29    |  85 |
| LED2        |  O  | PA28    |  84 |
