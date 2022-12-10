# stm32f072x8 / stm32f072xb HIC

Based on STM32F072C8T6 chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32f072c8.pdf)) and STM32F072CBT6 chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32f072cb.pdf)).

- Cortex-M0 48 Mhz
- 64 KB (stm32f072x8) / 128 KB (stm32f072xb)  Flash
- 20 KB RAM
- Full-speed USB 2.0 device controller: up to 8 bi-directional endpoints including EP0 (*)
- UFQFPN48 packaging

(*) "Configurable number of endpoints from 1 to 8", "up to 16 mono-directional or 8 bidirectional endpoints can be used". (source: [RM0008](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf))

## Memory Map

### stm32f072x8

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 64 KB  | 0x0000_0000 | 0x0001_0000 |
| SRAM     | 16 KB  | 0x2000_0000 | 0x2000_4000 |

### stm32f072xb

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0000_0000 | 0x0002_0000 |
| SRAM     | 16 KB  | 0x2000_0000 | 0x2000_4000 |

Bootloader size is 48 KB

## DAPLink default pin assignment

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD / JTAG  |
| SWCLK / TCK |  O  | PA2     |     |
| SWDIO / TMS | I/O | PA4     |     |
| SWO / TDO   |  I  | PA3     |     |
| nRESET      |  O  | PA1     |     |
| UART        |
| UART TX     |  O  | PA9     |     |
| UART RX     |  I  | PA10    |     |
| Button      |
| NF-RST But. |  I  | PB6     |     |
| LEDs        |
| Connect. LED|  O  | PA0     |     |
| HID LED     |  O  | PA0     |     |
| CDC LED     |  O  | PA0     |     |
| MSC LED     |  O  | PA0     |     |
