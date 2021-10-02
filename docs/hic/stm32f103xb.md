# stm32f103xb HIC

Based on STM32F103CBT6 chip ([Datasheet](https://www.st.com/resource/en/datasheet/stm32f103cb.pdf)):
- Cortex-M3 72 Mhz
- 128 KB Flash
- 20 KB RAM
- Full-speed USB 2.0 device controller
- UFQFPN48 packaging

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 128 KB | 0x0800_0000 | 0x0802_0000 |
| SRAM     | 20 KB  | 0x2000_0000 | 0x2000_5000 |

Bootloader size is 48 KB

## DAPLink default pin assignment 

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| SWD/JTAG    |
| SWCLK / TCK |  O  | PB13    |  26 |
| SWDIO / TMS |  O  | PB14    |  27 |
| SWDIO / TMS |  I  | PB12    |  25 |
| SWO / TDO   |  I  | PA10    |  31 |
| nRESET      |  O  | PB0     |  18 |
| UART (CDC)  |
| UART RX     |  I  | PA2     |  12 |
| UART TX     |  O  | PA3     |  13 |
| Button      |
| NF-RST But. |  I  | PB15    |  28 |
| LEDs        |
| Connect. LED|  O  | PB6     |  42 |
| HID LED     |  O  | PA9     |  30 |
| CDC LED     |  O  | PA9     |  30 |
| MSC LED     |  O  | PA9     |  30 |
