# stm32h743xx HIC

Based on STM32H743IIK6TR chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32h743ii.pdf)):
- Cortex-M7 480 Mhz
- 2 MBytes Flash
- 1 MByte RAM
- High-speed USB 2.0 device controller: up to 9 bi-directional endpoints including EP0 (*)
- UFBGA176+25 packaging

(*) "main features in peripheral-mode are the following: 1 bidirectional control endpoint0, 8 IN endpoints (EPs) configurable to support bulk, interrupt or isochronous transfers, 8 OUT endpoints configurable to support bulk, interrupt or isochronous transfers" (source: [RM0433](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf))

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 2048 KB| 0x0800_0000 | 0x1FF0_0000 |
| DTCM     | 128 KB | 0x2000_0000 | 0x2400_0000 |
| AXI-SRAM | 384 KB | 0x2400_0000 | 0x3000_0000 |
| SRAM1    | 32 KB  | 0x3000_0000 | 0x3002_0000 |
| SRAM2    | 16 KB  | 0x3002_0000 | 0x3800_0000 |
| SRAM4    | 64 KB  | 0x3800_0000 | 0x3880_0000 |

Bootloader size is 128 KB

## DAPLink default pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD / JTAG  |
| SWCLK / TCK |  O  | PD7     |  56 |
| SWDIO / TMS | I/O | PD2     |  51 |
| SWO / TDO   |  I  | PB7     |  24 |
| nRESET      |  O  | PB0     |  17 |
| UART        |
| UART RX     |  I  | PD9     |  58 |
| UART TX     |  O  | PD8     |  57 |
| Button      |
| NF-RST But. |  I  | PI4     | 132 |
| LEDs        |
| Connect. LED|  O  | PG0     |  96 |
| HID LED     |  O  | PG2     |  98 |
| CDC LED     |  O  | PG3     |  99 |
| MSC LED     |  O  | PG4     | 100 |

## UDB (Universal Debug Board) Additions
UDB is a special flavor of the stm32h743xx HIC, which enriches the DAPLink experience with additional features such as:
- A new USB CDC endpoint to flash and get logs from a second target.
- Another new USB CDC endpoint to interact with the HIC. Users can send easy shell commands to toggle pins, or to get any info from the HIC.
- Additional DAPLink debugging functionality: persistent fault info, breakpoints on asserts when debugger is connected to debug UDB, watchdog, backtraces
- Target power measurement
