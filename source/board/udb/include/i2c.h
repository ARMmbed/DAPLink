#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>
#include "udb_errno.h"
#include "I2C_STM32H7xx.h"

typedef enum {
    I2C_BUS_0,
    I2C_BUS_1,
    I2C_BUS_2,
    I2C_BUS_COUNT,
} i2c_bus_t;

typedef struct
{
    i2c_bus_t bus_id;
    uint16_t slave_addr;
} i2c_slave_t;

// Setup up I2C bus and enables interrupts
int i2c_init(void);

void i2c_request(const i2c_slave_t *i2c_slave);
void i2c_release(const i2c_slave_t *i2c_slave);

/*
 * i2c_write
 *
 * i2c_slave: Target (slave) device to be communicated with
 * reg_addr: Target device register to start writing to
 * in_buf: bytes to be written to target device
 * len: length of data, or number of bytes to be sequentially written
 *
 * Write sequential bytes, starting at given register address, on target I2C device
 */
int i2c_write(const i2c_slave_t *i2c_slave, uint8_t reg_addr, const uint8_t* in_buf, uint32_t len);

/*
 * i2c_read
 *
 * i2c_slave: Target (slave) device to be communicated with
 * reg_addr: Target device register to start reading from
 * out_buf: buffer to store data read from target device
 * len: number of bytes to be read
 *
 * Read sequential bytes, starting at given register address, on target I2C device
 */
int i2c_read(const i2c_slave_t *i2c_slave, uint8_t reg_addr, uint8_t* out_buf, uint32_t len);

#endif /* I2C_H */
