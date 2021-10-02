# lpc55xx HIC

Based on LPC55S69JBD64 chip ([Data Sheet](https://www.nxp.com/docs/en/data-sheet/LPC55S6x.pdf)):
- Cortex-M33 150 Mhz (cores)
- 640 KB Flash
- 320 KB RAM
- High-speed USB 2.0 host/device controller
- HTQFP64 packaging

In order to support other MCUs from the LPC5500 family, the HIC limits usage
to one core, 320 KB Flash and 256 KB RAM.

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 320 KB | 0x0000_0000 | 0x0005_1000 |
| SRAM     | 256 KB | 0x2000_0000 | 0x2004_0000 |
| USB RAM  |  16 KB | 0x4010_0000 | 0x4010_4000 |

Bootloader size is 64 KB

## DAPLink default pin assignment 

Reference design is based on [MCU-Link](https://www.nxp.com/design/development-boards/lpcxpresso-boards/mcu-link-debug-probe:MCU-LINK).

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD/JTAG    |
| SWCLK / TCK |  O  | PIO0_0  |  36 |
| SWDIO / TMS | I/O | PIO0_2  |  52 |
| SWO / TDO   |  I  | PIO0_3  |  53 |
| TDI         |  O  | PIO0_1  |   2 |
| nRESET      |  O  | PIO0_19 |  58 |
| GNDDetect   |  I  | PIO0_22 |  50 |
| UART        |
| UART RX     |  I  | PIO0_24 |  45 |
| UART TX     |  O  | PIO0_25 |  51 |
| Button      |
| NF-RST But. |  I  | PIO0_18 |  38 |
| F-RST  But. |  I  | PIO1_27 |  18 |
| LEDs        |
| Con. LED    |  O  | PIO0_5  |  56 |

Additional pins are used to drive external circuitry:
- SWDIO TX Enable (PIO0_28, pin 44)
- nRESET Output Enable (PIO0_13, pin 46)
