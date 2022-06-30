#include "shell_cmd_i2c.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define I2C_ARGC_READ                   (5)
#define I2C_ARGC_WRITE                  (I2C_ARGC_READ + 1)

// range for 7-bit addressing
#define I2C_SLAVE_ADDR_MIN              (0x08)
#define I2C_SLAVE_ADDR_MAX              (0x77)
#define UDB_I2C_CMD_READ_BUFFER_SIZE    (32)
#define I2C_WRITE_MAX_LEN               (sizeof(uint32_t) / sizeof(uint8_t))

void cmd_i2c(int argc, char *argv[])
{
    if (strcmp(argv[1], "probe") == 0)
    {
        printf("probing...\n");

        uint8_t bus_id = strtoul(argv[2], NULL, 0);

        if (bus_id < I2C_BUS_COUNT)
        {
            i2c_slave_t slave =
            {
                .bus_id = bus_id,
            };

            i2c_request(&slave);
            for (uint16_t addr = I2C_SLAVE_ADDR_MIN; addr <= I2C_SLAVE_ADDR_MAX; ++addr)
            {
                slave.slave_addr = addr;
                if (i2c_write(&slave, 0, 0, 0) == UDB_SUCCESS)
                {
                    printf("%#.*x\n", 2, addr);
                }
            }
            i2c_release(&slave);
        }
        else
        {
            printf("Error: UDB doesn't have bus %u\n", bus_id);
        }
    }
    else if ((argc == I2C_ARGC_READ) || (argc == I2C_ARGC_WRITE))
    {
        uint8_t bus_id = strtoul(argv[1], NULL, 0);
        uint16_t slave_addr = strtoul(argv[2], NULL, 0);
        uint8_t start_reg = strtoul(argv[3], NULL, 0);
        uint16_t len = strtoul(argv[4], NULL, 0);

        i2c_slave_t slave =
        {
            .bus_id = bus_id,
            .slave_addr = slave_addr,
        };
        if (bus_id < I2C_BUS_COUNT)
        {
            if (argc == I2C_ARGC_READ)
            {
                printf("reading..\n");

                uint8_t buffer[UDB_I2C_CMD_READ_BUFFER_SIZE];

                if (len > UDB_I2C_CMD_READ_BUFFER_SIZE)
                {
                    len = UDB_I2C_CMD_READ_BUFFER_SIZE;
                    printf("Error: max buffer size is %u\n", UDB_I2C_CMD_READ_BUFFER_SIZE);
                }

                i2c_request(&slave);

                if (i2c_read(&slave, start_reg, buffer, len) == UDB_SUCCESS)
                {
                    for (int i = 0; i < len; ++i)
                    {
                        printf("%d:0x%x\n", i, buffer[i]);
                    }
                }
                else
                {
                    printf("Error: i2c read failed\n");
                }

                i2c_release(&slave);
            }
            else
            {
                printf("writing...\n");

                uint32_t val = strtoul(argv[5], NULL, 0);

                if (len > I2C_WRITE_MAX_LEN)
                {
                    len = I2C_WRITE_MAX_LEN;
                    printf("Error: max buffer size is %u\n", I2C_WRITE_MAX_LEN);
                }

                i2c_request(&slave);

                if (i2c_write(&slave, start_reg, (uint8_t *)&val, len) != UDB_SUCCESS)
                {
                    printf("Error: i2c write failed\n");
                }

                i2c_release(&slave);
            }
        }
        else
        {
            printf("Error: UDB doesn't have bus %u\n", bus_id);
        }
    }
    else
    {
        printf("Error: Invalid i2c command\n");
    }
}
