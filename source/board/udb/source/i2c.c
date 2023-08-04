#include "i2c.h"
#include "util.h"

#include <string.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "util.h"

#define XFER_PENDING        (true)
#define XFER_NOT_PENDING    (false)

typedef struct
{
    ARM_DRIVER_I2C*       driver;
    ARM_I2C_SignalEvent_t event_cb;
    IRQn_Type             irq_n;
} i2c_arm_driver_context_t;

typedef struct
{
    const i2c_arm_driver_context_t driver_context;

    I2C_HandleTypeDef*    handle;
    uint32_t              event;
    osMutexId_t           mutex_id;
    osMutexAttr_t         mutex_attr;
    uint32_t              mutex_cb[WORDS(sizeof(osRtxMutex_t))];
} i2c_context_t;

// Define handles for ARM I2C driver
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

static void i2c1_event_cb (uint32_t event);
static void i2c2_event_cb (uint32_t event);
static void i2c3_event_cb (uint32_t event);

static i2c_context_t s_i2c_context[I2C_BUS_COUNT] =
{
    [I2C_BUS_0] =
    {
        .driver_context =
        {
            .driver = &Driver_I2C1,
            .event_cb = i2c1_event_cb,
            .irq_n = I2C1_EV_IRQn,
        },
        .handle = &hi2c1,
        .event = 0,
        .mutex_attr =
        {
            .name = "i2c0_mutex",
            .attr_bits = osMutexRobust,
            .cb_mem = s_i2c_context[I2C_BUS_0].mutex_cb,
            .cb_size = sizeof(s_i2c_context[I2C_BUS_0].mutex_cb),
        },
    },
    [I2C_BUS_1] =
    {
        .driver_context =
        {
            .driver = &Driver_I2C3,
            .event_cb = i2c3_event_cb,
            .irq_n = I2C3_EV_IRQn,
        },
        .handle = &hi2c3,
        .event = 0,
        .mutex_attr =
        {
            .name = "i2c1_mutex",
            .attr_bits = osMutexRobust,
            .cb_mem = s_i2c_context[I2C_BUS_1].mutex_cb,
            .cb_size = sizeof(s_i2c_context[I2C_BUS_1].mutex_cb),
        },
    },
    [I2C_BUS_2] =
    {
        .driver_context =
        {
            .driver = &Driver_I2C2,
            .event_cb = i2c2_event_cb,
            .irq_n = I2C2_EV_IRQn,
        },
        .handle = &hi2c2,
        .event = 0,
        .mutex_attr =
        {
            .name = "i2c2_mutex",
            .attr_bits = osMutexRobust,
            .cb_mem = s_i2c_context[I2C_BUS_2].mutex_cb,
            .cb_size = sizeof(s_i2c_context[I2C_BUS_2].mutex_cb),
        },
    },
};

/*
 * Callback for ARM's I2C driver
 */
static void i2c1_event_cb (uint32_t event)
{
    if (event != ARM_I2C_EVENT_BUS_CLEAR)
    {
        s_i2c_context[I2C_BUS_0].event |= event;
    }
}

static void i2c3_event_cb (uint32_t event)
{
    if (event != ARM_I2C_EVENT_BUS_CLEAR)
    {
        s_i2c_context[I2C_BUS_1].event |= event;
    }
}

static void i2c2_event_cb (uint32_t event)
{
    if (event != ARM_I2C_EVENT_BUS_CLEAR)
    {
        s_i2c_context[I2C_BUS_2].event |= event;
    }
}

/*
 * IRQHandler: interrupt handler, which calls the HAL_I2C interrupt handler
 */
void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(s_i2c_context[I2C_BUS_0].handle);
}

void I2C2_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(s_i2c_context[I2C_BUS_2].handle);
}

void I2C3_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(s_i2c_context[I2C_BUS_1].handle);
}

int i2c_init(void)
{
    int ret = UDB_SUCCESS;
    ARM_DRIVER_I2C* i2c_drv;

    for (i2c_bus_t bus_id = 0; bus_id < I2C_BUS_COUNT; ++bus_id)
    {
        s_i2c_context[bus_id].mutex_id = osMutexNew(&s_i2c_context[bus_id].mutex_attr);
        if (s_i2c_context[bus_id].mutex_id  == NULL)
        {
            util_assert(false);
        }
        i2c_drv = s_i2c_context[bus_id].driver_context.driver;

        i2c_drv->Initialize(s_i2c_context[bus_id].driver_context.event_cb);

        if (i2c_drv->PowerControl(ARM_POWER_FULL) != ARM_DRIVER_OK)
        {
            ret = -UDB_ERROR;
            break;
        }

        if (i2c_drv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD) != ARM_DRIVER_OK)
        {
            ret = -UDB_ERROR;
            break;
        }

        if (i2c_drv->Control(ARM_I2C_BUS_CLEAR, 0) != ARM_DRIVER_OK)
        {
            ret = -UDB_ERROR;
            break;
        }

        NVIC_SetPriority(s_i2c_context[bus_id].driver_context.irq_n, 1);
        NVIC_EnableIRQ(s_i2c_context[bus_id].driver_context.irq_n);
    }

    return ret;
}

void i2c_request(const i2c_slave_t *i2c_slave)
{
    util_assert(i2c_slave != NULL);
    util_assert(i2c_slave->bus_id < I2C_BUS_COUNT);

    if (osMutexAcquire(s_i2c_context[i2c_slave->bus_id].mutex_id, osWaitForever) != osOK)
    {
        util_assert(false);
    }
}

void i2c_release(const i2c_slave_t *i2c_slave)
{
    util_assert(i2c_slave != NULL);
    util_assert(i2c_slave->bus_id < I2C_BUS_COUNT);

    if (osMutexRelease(s_i2c_context[i2c_slave->bus_id].mutex_id) != osOK)
    {
        util_assert(false);
    }
}

int i2c_write(const i2c_slave_t *i2c_slave, uint8_t reg_addr, const uint8_t* in_buf, uint32_t len)
{
    util_assert(i2c_slave != NULL);
    util_assert(i2c_slave->bus_id < I2C_BUS_COUNT);

    int             ret = UDB_SUCCESS;
    int32_t         arm_driver_status;
    i2c_bus_t       bus_id = i2c_slave->bus_id;
    uint16_t        slave_addr = i2c_slave->slave_addr;
    ARM_DRIVER_I2C* i2c_drv = s_i2c_context[i2c_slave->bus_id].driver_context.driver;
    uint8_t         transfer_data[len+1];

    transfer_data[0] = reg_addr;
    memcpy(transfer_data + 1, in_buf, len);

    /* Single write transfer of slave address, register address and data to be written */

    arm_driver_status = i2c_drv->MasterTransmit(slave_addr, transfer_data, len + 1, XFER_NOT_PENDING);

    if (arm_driver_status == ARM_DRIVER_ERROR_BUSY)
    {
        ret = -UDB_BUSY;
        goto done;
    }
    else if (arm_driver_status != ARM_DRIVER_OK)
    {
        ret = -UDB_ERROR;
        goto done;
    }

    /* Wait until transfer completed */
    while (i2c_drv->GetStatus().busy) { }

    if (s_i2c_context[bus_id].event != ARM_I2C_EVENT_TRANSFER_DONE)
    {
        ret = -UDB_ERROR;
    }

done:
    s_i2c_context[bus_id].event = 0;

    return ret;
}

int i2c_read(const i2c_slave_t *i2c_slave, uint8_t reg_addr, uint8_t* out_buf, uint32_t len)
{
    util_assert(i2c_slave != NULL);
    util_assert(i2c_slave->bus_id < I2C_BUS_COUNT);

    int             ret = UDB_SUCCESS;
    int32_t         arm_driver_status;
    i2c_bus_t       bus_id = i2c_slave->bus_id;
    uint16_t        slave_addr = i2c_slave->slave_addr;
    ARM_DRIVER_I2C* i2c_drv = s_i2c_context[bus_id].driver_context.driver;


    /* Send slave address and device address without stop command at end */
    arm_driver_status = i2c_drv->MasterTransmit(slave_addr, &reg_addr, 1, XFER_PENDING);

    if (arm_driver_status == ARM_DRIVER_ERROR_BUSY)
    {
        ret = -UDB_BUSY;
        goto done;
    }
    else if (arm_driver_status != ARM_DRIVER_OK)
    {
        ret = -UDB_ERROR;
        goto done;
    }

    /* Wait until transfer completed */
    while (i2c_drv->GetStatus().busy) { }

    /* Check if all data transferred */
    if (s_i2c_context[bus_id].event != ARM_I2C_EVENT_TRANSFER_DONE)
    {
        ret = -UDB_ERROR;
        goto done;
    }

    s_i2c_context[bus_id].event = 0;

    /* Send slave address and read from register address with stop command at end */
    arm_driver_status = i2c_drv->MasterReceive(slave_addr, out_buf, len, XFER_NOT_PENDING);

    if (arm_driver_status == ARM_DRIVER_ERROR_BUSY)
    {
        ret = -UDB_BUSY;
        goto done;
    }
    else if (arm_driver_status != ARM_DRIVER_OK)
    {
        ret = -UDB_ERROR;
        goto done;
    }

    /* Wait until transfer completed */
    while (i2c_drv->GetStatus().busy) { }

    if (s_i2c_context[bus_id].event != ARM_I2C_EVENT_TRANSFER_DONE)
    {
        ret = -UDB_ERROR;
    }


done:
    s_i2c_context[bus_id].event = 0;

    return ret;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }

    if (hi2c->Instance == I2C2)
    {
        __HAL_RCC_I2C2_CLK_ENABLE();
    }

    if (hi2c->Instance == I2C3)
    {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        __HAL_RCC_I2C1_CLK_DISABLE();
    }

    if (hi2c->Instance == I2C2)
    {
        __HAL_RCC_I2C2_CLK_DISABLE();
    }

    if (hi2c->Instance == I2C3)
    {
        __HAL_RCC_I2C3_CLK_DISABLE();
    }
}
