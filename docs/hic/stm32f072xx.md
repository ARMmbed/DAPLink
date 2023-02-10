# stm32f072x8 / stm32f072xb HIC

Based on STM32F072C8T6 chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32f072c8.pdf)) and STM32F072CBT6 chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32f072cb.pdf)).

- Cortex-M0 48 Mhz
- 64 KB (stm32f072x8) / 128 KB (stm32f072xb) Flash
- 16 KB RAM
- Full-speed USB 2.0 device controller: up to 8 bi-directional endpoints.
- UFQFPN48 packaging

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

Default pin-out is compatible with [J-Link-OB-STM32F072](https://www.segger.com/downloads/jlink/UM08024_JLinkOBSTM32F072.pdf).

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD         |
| SWCLK       |  O  | PA2     |  12 |
| SWDIO       | I/O | PA4     |  14 |
| SWO         |  I  | PA3     |  13 |
| nRESET      |  O  | PA1     |  11 |
| UART        |
| UART TX     |  O  | PA9     |  30 |
| UART RX     |  I  | PA10    |  31 |
| Button      |
| F-RST But.  |  I  | PB6     |     |
| LEDs        |
| Connect. LED|  O  | PA0     |     |
| HID LED     |  O  | PA0     |     |
| CDC LED     |  O  | PA0     |     |
| MSC LED     |  O  | PA0     |     |
