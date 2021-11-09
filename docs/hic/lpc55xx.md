# lpc55xx HIC

Based on LPC55S69JBD64 chip ([Data Sheet](https://www.nxp.com/docs/en/data-sheet/LPC55S6x.pdf)):
- Cortex-M33 150 Mhz (cores)
- 640 KB Flash
- 320 KB RAM
- High-speed USB 2.0 host/device controller
- HTQFP64 packaging

In order to support other MCUs from the LPC5500 family, the HIC limits usage
to one core, 256 KB Flash and 96 KB RAM.

## Memory Map

| Region   |  Size  | Start       | End         |
|----------|--------|-------------|-------------|
| Flash    | 256 KB | 0x0000_0000 | 0x0004_0000 |
| SRAM     |  96 KB | 0x2000_0000 | 0x2004_0000 |
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
| nRESET      | I/O | PIO0_19 |  58 |
| GNDDetect   |  I  | PIO0_22 |  50 |
| UART        |
| UART RX     |  I  | PIO0_24 |  45 |
| UART TX     |  O  | PIO0_25 |  51 |
| LEDs        |
| Con. LED    |  O  | PIO0_5  |  56 |

Additional pins are used to drive external circuitry:
- SWDIO TX Enable (PIO0_28, pin 44)
- nRESET Output Enable (PIO0_13, pin 46)

## MCU-LINK and MCU-LINK-PRO support

The `lpc55s69_mculink_bl` and `lpc55s69_mculink_if` images support both
MCU-LINK and MCU-LINK-PRO boards. Installation of the bootloader can be
done by putting the board in firmware update mode and using the NXP
`blhost` tool:

```
$ blhost --usb 0x1fc9,0x0021 flash-image lpc55s69_mculink_bl_crc.hex erase
```

### MCU-LINK "Buttoms"

The MCU-LINK does not have amy buttons, but both board have "Disable VCOM"
and "Disable SWD" respectively mapped to non-forwarding reset and
forwarding reset functionality in DAPLink.

| Signal      | I/O | Symbol  | Pin |
|-------------|:---:|---------|:---:|
| Button      |
| NF-RST But. |  I  | PIO0_18 |  38 |
| F-RST  But. |  I  | PIO0_27 |  18 |

### MCU-LINK-PRO detection and LEDs

The [MCU-LINK-PRO](https://www.nxp.com/design/microcontrollers-developer-resources/mcu-link-pro-debug-probe:MCU-LINK-PRO)
defines board type detection using `HW_VER_4` and `HW_VER_3`. When
`HW_VER_4` and `HW_VER_3` are respectively low and high, we determine that
the board is a MCU-LINK-PRO, otherwise we assume the board is a MCU-LINK.
MCU-LINK only has one red activity LED. MCU-LINK-PRO has 7 LEDs (1 red
and 6 green), 5 are currently used by DAPLink (see table below).

| Signal        | I/O | Symbol  |
|---------------|:---:|---------|
| Detection     |
| HW_VER_4      |  I  | PIO1_5  |
| HW_VER_3      |  I  | PIO1_4  |
| LEDs          |
| Con. LED  (r) |  O  | PIO0_5  |
| Run. LED  (g) |  O  | PIO1_17 |
| HID LED   (g) |  O  | PIO1_13 |
| CDC LED   (g) |  O  | PIO1_18 |
| MSC LED   (g) |  O  | PIO1_15 |
| N/A LED 1 (g) |  O  | PIO1_12 |
| N/A LED 2 (g) |  O  | PIO1_12 |
