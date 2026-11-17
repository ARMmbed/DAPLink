    .syntax unified
    .arch armv7e-m

    .section .stack, "aw", %nobits
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x00001000
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap, "aw", %nobits
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x00000400
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .if    Heap_Size
    .space    Heap_Size
    .endif
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External Interrupts (STM32F411xE) */
    .long    WWDG_IRQHandler
    .long    PVD_IRQHandler
    .long    TAMP_STAMP_IRQHandler
    .long    RTC_WKUP_IRQHandler
    .long    FLASH_IRQHandler
    .long    RCC_IRQHandler
    .long    EXTI0_IRQHandler
    .long    EXTI1_IRQHandler
    .long    EXTI2_IRQHandler
    .long    EXTI3_IRQHandler
    .long    EXTI4_IRQHandler
    .long    DMA1_Stream0_IRQHandler
    .long    DMA1_Stream1_IRQHandler
    .long    DMA1_Stream2_IRQHandler
    .long    DMA1_Stream3_IRQHandler
    .long    DMA1_Stream4_IRQHandler
    .long    DMA1_Stream5_IRQHandler
    .long    DMA1_Stream6_IRQHandler
    .long    ADC_IRQHandler
    .long    0
    .long    0
    .long    0
    .long    0
    .long    EXTI9_5_IRQHandler
    .long    TIM1_BRK_TIM9_IRQHandler
    .long    TIM1_UP_TIM10_IRQHandler
    .long    TIM1_TRG_COM_TIM11_IRQHandler
    .long    TIM1_CC_IRQHandler
    .long    TIM2_IRQHandler
    .long    TIM3_IRQHandler
    .long    TIM4_IRQHandler
    .long    I2C1_EV_IRQHandler
    .long    I2C1_ER_IRQHandler
    .long    I2C2_EV_IRQHandler
    .long    I2C2_ER_IRQHandler
    .long    SPI1_IRQHandler
    .long    SPI2_IRQHandler
    .long    USART1_IRQHandler
    .long    USART2_IRQHandler
    .long    0
    .long    EXTI15_10_IRQHandler
    .long    RTC_Alarm_IRQHandler
    .long    OTG_FS_WKUP_IRQHandler
    .long    0
    .long    0
    .long    0
    .long    0
    .long    DMA1_Stream7_IRQHandler
    .long    0
    .long    SDIO_IRQHandler
    .long    TIM5_IRQHandler
    .long    SPI3_IRQHandler
    .long    0
    .long    0
    .long    0
    .long    0
    .long    DMA2_Stream0_IRQHandler
    .long    DMA2_Stream1_IRQHandler
    .long    DMA2_Stream2_IRQHandler
    .long    DMA2_Stream3_IRQHandler
    .long    DMA2_Stream4_IRQHandler
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    OTG_FS_IRQHandler
    .long    DMA2_Stream5_IRQHandler
    .long    DMA2_Stream6_IRQHandler
    .long    DMA2_Stream7_IRQHandler
    .long    USART6_IRQHandler
    .long    I2C3_EV_IRQHandler
    .long    I2C3_ER_IRQHandler
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    0
    .long    FPU_IRQHandler
    .long    0
    .long    0
    .long    SPI4_IRQHandler
    .long    SPI5_IRQHandler

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/* Copy data from ROM to RAM */
    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__
.L_loop1:
    cmp    r2, r3
    ittt    lt
    ldrlt    r0, [r1], #4
    strlt    r0, [r2], #4
    blt    .L_loop1

/* Zero BSS */
    ldr    r1, =__bss_start__
    ldr    r2, =__bss_end__
    movs    r0, 0
.L_loop3:
    cmp    r1, r2
    itt    lt
    strlt    r0, [r1], #4
    blt    .L_loop3

    bl    SystemInit
    bl    _start

    .pool
    .size Reset_Handler, . - Reset_Handler

    .align 1
    .thumb_func
    .weak Default_Handler
    .type Default_Handler, %function
Default_Handler:
    b .
    .size Default_Handler, . - Default_Handler

    .macro def_irq_handler handler_name
    .weak \handler_name
    .set \handler_name, Default_Handler
    .endm

    def_irq_handler NMI_Handler
    def_irq_handler HardFault_Handler
    def_irq_handler MemManage_Handler
    def_irq_handler BusFault_Handler
    def_irq_handler UsageFault_Handler
    def_irq_handler SVC_Handler
    def_irq_handler DebugMon_Handler
    def_irq_handler PendSV_Handler
    def_irq_handler SysTick_Handler
    def_irq_handler WWDG_IRQHandler
    def_irq_handler PVD_IRQHandler
    def_irq_handler TAMP_STAMP_IRQHandler
    def_irq_handler RTC_WKUP_IRQHandler
    def_irq_handler FLASH_IRQHandler
    def_irq_handler RCC_IRQHandler
    def_irq_handler EXTI0_IRQHandler
    def_irq_handler EXTI1_IRQHandler
    def_irq_handler EXTI2_IRQHandler
    def_irq_handler EXTI3_IRQHandler
    def_irq_handler EXTI4_IRQHandler
    def_irq_handler DMA1_Stream0_IRQHandler
    def_irq_handler DMA1_Stream1_IRQHandler
    def_irq_handler DMA1_Stream2_IRQHandler
    def_irq_handler DMA1_Stream3_IRQHandler
    def_irq_handler DMA1_Stream4_IRQHandler
    def_irq_handler DMA1_Stream5_IRQHandler
    def_irq_handler DMA1_Stream6_IRQHandler
    def_irq_handler ADC_IRQHandler
    def_irq_handler EXTI9_5_IRQHandler
    def_irq_handler TIM1_BRK_TIM9_IRQHandler
    def_irq_handler TIM1_UP_TIM10_IRQHandler
    def_irq_handler TIM1_TRG_COM_TIM11_IRQHandler
    def_irq_handler TIM1_CC_IRQHandler
    def_irq_handler TIM2_IRQHandler
    def_irq_handler TIM3_IRQHandler
    def_irq_handler TIM4_IRQHandler
    def_irq_handler I2C1_EV_IRQHandler
    def_irq_handler I2C1_ER_IRQHandler
    def_irq_handler I2C2_EV_IRQHandler
    def_irq_handler I2C2_ER_IRQHandler
    def_irq_handler SPI1_IRQHandler
    def_irq_handler SPI2_IRQHandler
    def_irq_handler USART1_IRQHandler
    def_irq_handler USART2_IRQHandler
    def_irq_handler EXTI15_10_IRQHandler
    def_irq_handler RTC_Alarm_IRQHandler
    def_irq_handler OTG_FS_WKUP_IRQHandler
    def_irq_handler DMA1_Stream7_IRQHandler
    def_irq_handler SDIO_IRQHandler
    def_irq_handler TIM5_IRQHandler
    def_irq_handler SPI3_IRQHandler
    def_irq_handler DMA2_Stream0_IRQHandler
    def_irq_handler DMA2_Stream1_IRQHandler
    def_irq_handler DMA2_Stream2_IRQHandler
    def_irq_handler DMA2_Stream3_IRQHandler
    def_irq_handler DMA2_Stream4_IRQHandler
    def_irq_handler OTG_FS_IRQHandler
    def_irq_handler DMA2_Stream5_IRQHandler
    def_irq_handler DMA2_Stream6_IRQHandler
    def_irq_handler DMA2_Stream7_IRQHandler
    def_irq_handler USART6_IRQHandler
    def_irq_handler I2C3_EV_IRQHandler
    def_irq_handler I2C3_ER_IRQHandler
    def_irq_handler FPU_IRQHandler
    def_irq_handler SPI4_IRQHandler
    def_irq_handler SPI5_IRQHandler

    .end
