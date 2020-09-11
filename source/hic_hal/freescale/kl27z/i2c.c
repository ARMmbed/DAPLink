#include "i2c.h"
#include "fsl_i2c.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "settings.h" // for config_get_overflow_detect

/* I2C source clock */
#define I2C_SLAVE_BASEADDR I2C1
#define I2C_SLAVE_CLK_SRC I2C1_CLK_SRC
#define I2C_SLAVE_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)

#define I2C_SLAVE_LOWER_ADDR_7BIT   (0x70U)
#define I2C_SLAVE_UPPER_ADDR_7BIT   (0x72U)
#define I2C_DATA_LENGTH             (1024U + 8U)

// We'll start with 5 RX commands
#define RX_CMDS_LENGTH              5
#define I2C_CMD_SLEEP               0xABU

static uint8_t g_slave_TX_buff[I2C_DATA_LENGTH];
static uint8_t g_slave_RX_buff[I2C_DATA_LENGTH];
i2c_slave_handle_t g_s_handle;
static volatile bool g_SlaveCompletionFlag = false;
static volatile bool g_SlaveRxFlag = false;
uint8_t address_match = 0;

static i2cWriteCallback_t pfWriteCommsCallback = NULL;
static i2cReadCallback_t pfReadCommsCallback = NULL;
static i2cWriteCallback_t pfWriteHIDCallback = NULL;
static i2cReadCallback_t pfReadHIDCallback = NULL;
static i2cWriteCallback_t pfWriteFlashCallback = NULL;
static i2cReadCallback_t pfReadFlashCallback = NULL;

static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData) {
    switch (xfer->event)
    {
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            xfer->data     = NULL;
            xfer->dataSize = 0;
            // fsl_i2c.c IRQ updated in fsl_i2c_mod.c to include I2C D register
            address_match = *(uint8_t*)userData >> 1;
            break;
        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data     = g_slave_TX_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            // Hack: Default driver can't differentiate between RX or TX on
            // completion event, so we set a flag here. Can't process more
            // than I2C_DATA_LENGTH bytes on RX
            memset(&g_slave_RX_buff, 0, sizeof(g_slave_RX_buff));
            xfer->data     = g_slave_RX_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            g_SlaveRxFlag = true;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            xfer->data            = NULL;
            xfer->dataSize        = 0;
            
            // Default driver couldn't differentiate between RX or TX completion
            // Check flag set in kI2C_SlaveReceiveEvent
            if (g_SlaveRxFlag) {
                g_SlaveRxFlag = false;
                
                if (pfWriteCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
                    pfWriteCommsCallback(&g_slave_RX_buff[0], xfer->transferredCount);
                }
                if (pfWriteHIDCallback && address_match == I2C_SLAVE_HID) {
                    pfWriteHIDCallback(&g_slave_RX_buff[0], xfer->transferredCount);
                }
                if (pfWriteFlashCallback && address_match == I2C_SLAVE_FLASH) {
                    pfWriteFlashCallback(&g_slave_RX_buff[0], xfer->transferredCount);
                }
            } else { 
                if (pfReadCommsCallback && address_match == I2C_SLAVE_NRF_KL_COMMS) {
                    pfReadCommsCallback(&g_slave_TX_buff[0], xfer->transferredCount);
                }
                if (pfReadHIDCallback && address_match == I2C_SLAVE_HID) {
                    pfReadHIDCallback(&g_slave_TX_buff[0], xfer->transferredCount);
                }
                if (pfReadFlashCallback && address_match == I2C_SLAVE_FLASH) {
                    pfReadFlashCallback(&g_slave_TX_buff[0], xfer->transferredCount);
                }
            }
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

static void i2c_init_pins(void) {
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC1 is configured as I2C1_SCL */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt2);

    PORTC->PCR[1] = ((PORTC->PCR[1] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTC2 is configured as I2C1_SDA */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt2);

    PORTC->PCR[2] = ((PORTC->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));
}

static int32_t i2c_start_transfer(void) {
    memset(&g_s_handle, 0, sizeof(g_s_handle));

    I2C_SlaveTransferCreateHandle(I2C_SLAVE_BASEADDR, &g_s_handle,
            i2c_slave_callback, &address_match);

    /* Set up slave transfer. */
    I2C_SlaveTransferNonBlocking(I2C_SLAVE_BASEADDR, &g_s_handle,
            kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent);
    
    // i2c handle will be busy until an address match
    g_s_handle.isBusy = false;

    return 1;
}

int32_t i2c_initialize(void) {
    i2c_slave_config_t slaveConfig;
    
    i2c_init_pins();

    I2C_SlaveGetDefaultConfig(&slaveConfig);

    slaveConfig.addressingMode = kI2C_RangeMatch;
    slaveConfig.slaveAddress   = I2C_SLAVE_LOWER_ADDR_7BIT;
    slaveConfig.upperAddress   = I2C_SLAVE_UPPER_ADDR_7BIT;
    slaveConfig.enableWakeUp   = true;

    I2C_SlaveInit(I2C_SLAVE_BASEADDR, &slaveConfig, I2C_SLAVE_CLK_FREQ);

    i2c_start_transfer();

    return 1;
}

int32_t i2c_deinitialize(void) {
    I2C_SlaveDeinit(I2C_SLAVE_BASEADDR);
    return 1;
}

status_t i2c_RegisterWriteCallback(i2cWriteCallback_t writeCallback, uint8_t slaveAddress)
{
    status_t status = kStatus_Success;
    
    switch (slaveAddress){
        case I2C_SLAVE_NRF_KL_COMMS:
            pfWriteCommsCallback = writeCallback;
            break;
        case I2C_SLAVE_HID:
            pfWriteHIDCallback = writeCallback;
            break;
        case I2C_SLAVE_FLASH:
            pfWriteFlashCallback = writeCallback;
            break;
        default:
            status = kStatus_Fail;
            break;
    }
    
    return status;
}

status_t i2c_RegisterReadCallback(i2cReadCallback_t readCallback, uint8_t slaveAddress)
{
    status_t status = kStatus_Success;
    
    switch (slaveAddress) {
        case I2C_SLAVE_NRF_KL_COMMS:
            pfReadCommsCallback = readCallback;
            break;
        case I2C_SLAVE_HID:
            pfReadHIDCallback = readCallback;
            break;
        case I2C_SLAVE_FLASH:
            pfReadFlashCallback = readCallback;
            break;
        default:
            status = kStatus_Fail;
            break;
    }
    
    return status;
}

void i2c_clearBuffer (void) {
    memset(&g_slave_TX_buff, 0, sizeof(g_slave_TX_buff));
}

void i2c_fillBuffer (uint8_t* data, uint32_t position, uint32_t size) {
    if ((position + size) > I2C_DATA_LENGTH) {
        return;
    }
    
    for (uint32_t i = 0; i < size; i++) { 
            g_slave_TX_buff[position + i] = data[i];
    }
}

