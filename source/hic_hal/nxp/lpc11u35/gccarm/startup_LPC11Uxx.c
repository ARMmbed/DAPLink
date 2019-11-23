/*================================*/
/*=====LPC11XX GNU STARTUP========*/
/*==A CODERED COMPATIBLE STARTUP==*/
/*================================*/
#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C"
{
  extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

#include "LPC11Uxx.h"

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

extern unsigned int __data_load_addr;
extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __StackTop;
#ifdef FIXED_STACKHEAP_SIZE
#define STACK_SIZE  (800)
#define HEAP_SIZE  (200)
unsigned char StackMem[STACK_SIZE] __attribute__ ((section(".stack")));
unsigned char HeapMem[HEAP_SIZE] __attribute__ ((section(".heap"), align(8)));
#endif
//*****************************************************************************
//
// Forward declaration of the default handlers. These are aliased.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions
//
//*****************************************************************************
     void Reset_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);
//*****************************************************************************
//
// Forward declaration of the specific IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the application
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//
//*****************************************************************************

void FLEX_INT0_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT1_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT2_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT3_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT4_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT5_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT6_IRQHandler (void) ALIAS(IntDefaultHandler);
void FLEX_INT7_IRQHandler (void) ALIAS(IntDefaultHandler);
void GINT0_IRQHandler     (void) ALIAS(IntDefaultHandler);
void GINT1_IRQHandler     (void) ALIAS(IntDefaultHandler);
void SSP1_IRQHandler      (void) ALIAS(IntDefaultHandler);
void I2C_IRQHandler       (void) ALIAS(IntDefaultHandler);
void TIMER16_0_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIMER16_1_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIMER32_0_IRQHandler (void) ALIAS(IntDefaultHandler);
void TIMER32_1_IRQHandler (void) ALIAS(IntDefaultHandler);
void SSP0_IRQHandler      (void) ALIAS(IntDefaultHandler);
void UART_IRQHandler      (void) ALIAS(IntDefaultHandler);
void USB_IRQHandler       (void) ALIAS(IntDefaultHandler);
void USB_FIQHandler       (void) ALIAS(IntDefaultHandler);
void ADC_IRQHandler       (void) ALIAS(IntDefaultHandler);
void WDT_IRQHandler       (void) ALIAS(IntDefaultHandler);
void BOD_IRQHandler       (void) ALIAS(IntDefaultHandler);
void FMC_IRQHandler       (void) ALIAS(IntDefaultHandler);
void USBWakeup_IRQHandler (void) ALIAS(IntDefaultHandler);

//*****************************************************************************
//
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#else
extern int main(void);
#endif

//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif

__attribute__ ((section(".isr_vector_table")))
void (* const Vectors[])(void) = {
#ifdef FIXED_STACKHEAP_SIZE
  (void (*)(void))(StackMem + STACK_SIZE),          // The initial stack pointer
#else
  (void (*)(void))&__StackTop,
#endif
  Reset_Handler,                    // The reset handler
  NMI_Handler,                      // The NMI handler
  HardFault_Handler,                // The hard fault handler
  0,                                // Reserved
  0,                                // Reserved
  0,                                // Reserved
  0,                                // Reserved
  0,                                // Reserved
  0,                                // Reserved
  0,                                // Reserved
  SVC_Handler,                      // SVCall handler
  0,                                // Reserved
  0,                                // Reserved
  PendSV_Handler,                   // The PendSV handler
  SysTick_Handler,                  // The SysTick handler

  // LPC11U specific handlers
  FLEX_INT0_IRQHandler,             //  0 - GPIO pin interrupt 0
  FLEX_INT1_IRQHandler,             //  1 - GPIO pin interrupt 1
  FLEX_INT2_IRQHandler,             //  2 - GPIO pin interrupt 2
  FLEX_INT3_IRQHandler,             //  3 - GPIO pin interrupt 3
  FLEX_INT4_IRQHandler,             //  4 - GPIO pin interrupt 4
  FLEX_INT5_IRQHandler,             //  5 - GPIO pin interrupt 5
  FLEX_INT6_IRQHandler,             //  6 - GPIO pin interrupt 6
  FLEX_INT7_IRQHandler,             //  7 - GPIO pin interrupt 7
  GINT0_IRQHandler,                 //  8 - GPIO GROUP0 interrupt
  GINT1_IRQHandler,                 //  9 - GPIO GROUP1 interrupt
  0,                                // 10 - Reserved
  0,                                // 11 - Reserved
  0,                                // 12 - Reserved
  0,                                // 13 - Reserved
  SSP1_IRQHandler,                  // 14 - SPI/SSP1 Interrupt
  I2C_IRQHandler,                   // 15 - I2C0
  TIMER16_0_IRQHandler,             // 16 - CT16B0 (16-bit Timer 0)
  TIMER16_1_IRQHandler,             // 17 - CT16B1 (16-bit Timer 1)
  TIMER32_0_IRQHandler,             // 18 - CT32B0 (32-bit Timer 0)
  TIMER32_1_IRQHandler,             // 19 - CT32B1 (32-bit Timer 1)
  SSP0_IRQHandler,                  // 20 - SPI/SSP0 Interrupt
  UART_IRQHandler,                  // 21 - UART0
  USB_IRQHandler,                   // 22 - USB IRQ
  USB_FIQHandler,                   // 23 - USB FIQ
  ADC_IRQHandler,                   // 24 - ADC (A/D Converter)
  WDT_IRQHandler,                   // 25 - WDT (Watchdog Timer)
  BOD_IRQHandler,                   // 26 - BOD (Brownout Detect)
  FMC_IRQHandler,                   // 27 - IP2111 Flash Memory Controller
  0,                                // 28 - Reserved
  0,                                // 29 - Reserved
  USBWakeup_IRQHandler,             // 30 - USB wake-up interrupt
  0,                                // 31 - Reserved
};

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void Reset_Handler(void)
{
  /*
   * Only Initialize Internal SRAM
   * USB RAM is used for USB purpose
   */
  unsigned int *src, *dst;
  
  // enable RAM1, RAM2, USB RAM
  unsigned int *sys_clk_ctrl = (unsigned int *)0x40048080;
  
  *sys_clk_ctrl = 0x0C00001F;

  /* Copy data section from flash to RAM */
  src = &__data_load_addr;
  dst = &__data_start;
  while (dst < &__data_end)
    *dst++ = *src++;

  /* Zero fill the bss section */
  dst = &__bss_start;
  while (dst < &__bss_end)
    *dst++ = 0;

  SystemInit();

#if defined (__cplusplus)
  //
  // Call C++ library initialisation
  //
  __libc_init_array();
#endif

#if defined (__REDLIB__)
  // Call the Redlib library, which in turn calls main()
  __main() ;
#else
  main();
#endif
  //
  // main() shouldn't return, but if it does, we'll just enter an infinite loop
  //
  while (1) {
    ;
  }
}

//*****************************************************************************
// Default exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void NMI_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void SVC_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void PendSV_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void SysTick_Handler(void)
{
    while(1)
    {
    }
}

//*****************************************************************************
//
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//
//*****************************************************************************
__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void)
{
    while(1)
    {
    }
}

