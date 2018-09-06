# DAPLINK HIC for STM32F072

# 1. GPIOs used by hic
## 1.1 LED
HID/CDC/MSC => PA0
## 1.2 SWD
SWD_DAT => PA4

SWD_CLK => PA2

SWD_NRST=> PA1

SWD_SWO => PA3  (RFU, not implemented yet)

BTN     => PB6  (to configure hic in vmsd)

## 1.3 USART
RTS     => PA6  (RFU, not implemented yet)

CTS     => PA7  (RFU, not implemented yet)

DTR     => PA8  (RFU, not implemented yet)

TX      => PA9

RX      => PA10


**see IO_Config.h for detail**

# 2. How to test

## 2.1 Make sure the following lines within your projects.yaml
```xml
module:
    hic_stm32f072cb: &module_hic_stm32f072cb
        - records/rtos/rtos-cm0.yaml
        - records/hic_hal/stm32f072cb.yaml
```

## 2.2 Add  your target in projects.yaml like:
```xml
projects:
    stm32f072cb_my_nrf52832_if:
        - *module_if
        - *module_hic_stm32f072cb
        - records/board/myboard.yaml
```

## 2.3 Generate Keil MDK project file
Follow the ¡°DAPLink Developers Guide¡± £¨docs/DEVELOPERS-GUIDE.md)

---
by dotnfc@163.com, 2018/09/01
