#include "DAP.h"
#include "settings.h"
#include <string.h>
#include "i2c.h"
#include "cortex_m.h"
#include "stm32h7xx.h"
#include "DAP_vendor_ex.h"
#include "DAP_config.h"
#include "udb_version.h"
#include "util.h"
#include "udb_adapter_detector.h"
#include "udb_reset.h"
#include "udb_power_measurement.h"
#include "udb_errno.h"

#define UDB_RESET_TIMER_MS   (500)

typedef struct
{
    int query_bit;
    dut_pin_group_id_t dut_pin_group_id;
} dut_pin_group_query_t;

static uint32_t DAP_ProcessVendorCommandEx40_MeasurePower(const uint8_t *request, uint8_t *response)
{
    udb_power_measurement_target_t last_target;

    if (udb_get_hw_version() < HW_VERSION_P3)
    {
        last_target = UDB_POWER_MEASUREMENT_TARGET_ADAPTER_USB;
    }
    else
    {
        last_target = UDB_POWER_MEASUREMENT_TARGET_DUT1;
    }

    // 2 bytes for voltage and 4 bytes for current
    util_assert(6 * (last_target + 1) <= DAP_PACKET_SIZE);

    uint32_t num = 0;

    if (udb_power_measurement_measure() != UDB_SUCCESS)
    {
        goto power_measurement_error;
    }

    uint8_t data_buf[DAP_PACKET_SIZE];
    uint8_t data_buf_idx = 0;

    for (udb_power_measurement_target_t target_type = 0; target_type <= last_target ; ++target_type)
    {
        uint16_t voltage_mV = 0;
        uint32_t current_uA = 0;

        // Proceed even when udb_power_measurement_ returns UDB_ERROR to read
        // all available channels.

        udb_power_measurement_read_voltage_mV(target_type, &voltage_mV);

        data_buf[data_buf_idx++] = (voltage_mV & 0xFF);
        data_buf[data_buf_idx++] = ((voltage_mV >> 8) & 0xFF);

        udb_power_measurement_read_current_uA(target_type, &current_uA);

        data_buf[data_buf_idx++] = (current_uA & 0xFF);
        data_buf[data_buf_idx++] = ((current_uA >> 8) & 0xFF);
        data_buf[data_buf_idx++] = ((current_uA >> 16) & 0xFF);
        data_buf[data_buf_idx++] = ((current_uA >> 24) & 0xFF);
    }

    *response++ = DAP_OK;

    memcpy(response, data_buf, data_buf_idx);
    response += data_buf_idx;

    num += 1 + data_buf_idx;

    return num;

power_measurement_error:
    *response = DAP_ERROR;
    num += 1;

    return num;
}

/** Process DAP Vendor Command from the Extended Command range and prepare Response Data
\param request   pointer to request data
\param response  pointer to response data
\return          number of bytes in response (lower 16 bits)
                 number of bytes in request (upper 16 bits)
*/
uint32_t DAP_ProcessVendorCommandEx(const uint8_t *request, uint8_t *response) {
  uint32_t num = (1U << 16) | 1U;  // count the Command ID byte

  *response++ = *request;        // copy Command ID

  switch (*request++) {          // first byte in request is Command ID
    case ID_DAP_VendorEx32_I2C_READ: {
        //  Read I2C data
        //
        //  This reads from the "I2C2" bus on the UDB board.  This is routed to an EEPROM
        //    (24AA02UIDT) at address 0x50, a voltage measurement chip (PAC1934T-I/J6CX)
        //    at address 0x17, and pins 13 and 15 of
        //    connector J1305 (with pullups to 1.8V).  It uses the MCU's internal I2C2 hw block.

        //  The inputs to this command are:
        //    I2C_TargetAddress, 1 byte (7 bit I2C chip address)
        //    I2C_starting_register_address, 1 byte
        //    Read_length, 1 byte (1 to 61 bytes is valid)
        //
        //  This returns a byte array which contains:
        //    status byte (0x00 = Success, 0xFF = Failure)
        //    I2C "event" status (2 bytes).  Search for "ARM_I2C_EVENT_TRANSFER_DONE" for bit definitions
        //      0x01 is "done", 0x10 is "cleared", other bits indicate errors.
        //    length byte
        //    1 to 61 data bytes
        //
        uint8_t target_addr = *request++;
        uint8_t internal_addr = *request++;
        uint8_t len = *request;
        uint8_t data_buf[64] = { 0 };
        int res;
        uint32_t returnVal;

        i2c_slave_t slave =
        {
            .bus_id = I2C_BUS_2,
            .slave_addr = target_addr,
        };


        i2c_request(&slave);
        res = i2c_read(&slave, internal_addr, data_buf, len);
        i2c_release(&slave);
        if (res == UDB_SUCCESS) {
            *response++ = DAP_OK;
            returnVal = 0;
        } else {
            // transfer incomplete or error
            *response++ = DAP_ERROR;
            returnVal = -res;
        }
        *response++ = (returnVal & 0xFF); //9 bits in an event.  Get first byte
        *response++ = ((returnVal >> 8) & 0xFF); //Get 2nd byte
        *response++ = len;  //length byte

        for (int i = 0; i < len; i++) {
            *response++ = data_buf[i];
        }

        //3 bytes read for the command, returns status byte, returnVal, length byte, and len data bytes
        num += (3 << 16) | (4 + len);
        break;
    }
    case ID_DAP_VendorEx33_I2C_WRITE: {
        //  Write I2C data
        //
        //  This writes data to the "I2C2" bus on the UDB board.  This is routed to an EEPROM
        //    (24AA02UIDT) at address 0x50, a voltage measurement chip (PAC1934T-I/J6CX)
        //    at address 0x17, and pins 13 and 15 of
        //    connector J1305 (with pullups to 1.8V).  It uses the MCU's internal I2C2 hw block.

        //  The inputs to this command are:
        //    I2C_TargetAddress, 1 byte (7 bit I2C chip address)
        //    I2C_starting_register_address, 1 byte
        //    Write_length, 1 byte (1 to 60 bytes is valid)
        //    Data, 1-60 bytes
        //
        //  This returns a byte array which contains:
        //    status byte (0x00 = Success, 0xFF = Failure)
        //    I2C "event" status (2 bytes).  Search for "ARM_I2C_EVENT_TRANSFER_DONE" for bit definitions
        //      0x01 is "done", 0x10 is "cleared", other bits indicate errors.
        //
        uint8_t target_addr = *request++;
        uint8_t internal_addr = *request++;
        uint8_t len = *request++;
        uint8_t data_buf[60] = {0};
        int res;
        uint32_t returnVal = 0;

        i2c_slave_t slave =
        {
            .bus_id = I2C_BUS_2,
            .slave_addr = target_addr,
        };

        for (int i = 0; i < len; i++) {
            data_buf[i] = *request++;
        }

        i2c_request(&slave);
        res = i2c_write(&slave, internal_addr, data_buf, len);
        i2c_release(&slave);

        if (returnVal == UDB_SUCCESS) {
            // transfer done
            *response++ = DAP_OK;
            returnVal = 0;
        } else {
            // transfer incomplete
            *response++ = DAP_ERROR;
            returnVal = -res;
        }

        *response++ = (returnVal & 0xFF); //9 bits in an event.  Get first byte
        *response++ = ((returnVal >> 8) & 0xFF); //Get 2nd byte

        //3 + len bytes read for the command, 3 bytes returned (status, returnVal)
        num += ((3 + len) << 16) | (3);
        break;
    }
    case ID_DAP_VendorEx34_DUT_PIN_GROUP_WRITE: {
        //  Write DUT group pins and also read back values.
        //
        //  This command has the following bytes:
        //    OUTPUT_VALUE, DIRECTION, MASK (optional)
        //
        //  OUTPUT_VALUE byte:
        //    bit 0 : 0_RST_L.  0 = pull low, 1 = pull high
        //    bit 1 : 0_BOOT_L
        //    bit 2 : 0_BTN_L
        //    bit 3 : RSVD
        //    bit 4 : 1_RST_H
        //    bit 5 : 1_BOOT_H
        //    bit 6 : 1_BTN_H
        //    bit 7 :  RSVD
        //
        //  DIRECTION byte:
        //    Same bit mapping as OUTPUT_VALUE. 0 = INPUT, 1 = OUTPUT
        //
        //  MASK byte:
        //    Same bit mapping as OUTPUT_VALUE. 0 = MASKED (don't write to that pin), 1 = write to that pin.
        //
        //  This command also reads the values from all the gpios (even if writing is masked)
        //  It returns 2 bytes, the first byte is the DIRECTION, the 2nd byte is the PIN_VALUE
        //  NOTE: If you just want to READ the pins, this command can be used with 0x00, 0x00, 0x00 (to mask writing to any pins)
        //
        uint8_t pin_values = *request++;            // When is an output, should it pull HIGH (1) or LOW (0)?  For
                                                    //  "open-drain" operation it should be 0 to pull low.  For
                                                    //  "push-pull" toggle between 1 and 0.
        uint8_t pin_direction = *request++;         // Should the level shifter drive the DUT (1), or be an input (0) from DUT?
        uint8_t pin_mask = *request;                // Masking byte, to allow indiviual pins to be set.  1=set,
                                                    //  0=don't change that pin.

        uint16_t pin_readback = 0;
        uint16_t dut_pin_query_bit;
        uint16_t dut_pin_query_mask;
        dut_pin_group_id_t dut_pin_group_id;

        const dut_pin_group_query_t k_dut_pin_group_queries[] =
        {
           {
              .query_bit = 0,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC0_RST_L,
           },
           {
              .query_bit = 1,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC0_BOOT_L,
           },
           {
              .query_bit = 2,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC0_BUTTON_L,
           },
           {
              .query_bit = 4,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC1_RST,
           },
           {
              .query_bit = 5,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC1_BOOT,
           },
           {
              .query_bit = 6,
              .dut_pin_group_id = DUT_PIN_GROUP_ID_UDC1_BUTTON,
           },
        };

        uint8_t query_count = sizeof(k_dut_pin_group_queries) / sizeof(k_dut_pin_group_queries[0]);

        for (uint8_t query_id = 0; query_id < query_count; ++query_id)
        {
            dut_pin_query_bit = k_dut_pin_group_queries[query_id].query_bit;
            dut_pin_query_mask = (1 << dut_pin_query_bit);
            dut_pin_group_id = k_dut_pin_group_queries[query_id].dut_pin_group_id;
            if (pin_mask & dut_pin_query_mask)
            {
                if (pin_direction & dut_pin_query_mask)
                {
                    if (pin_values & dut_pin_query_mask)
                    {
                        gpio_config_dut_pin_group(dut_pin_group_id, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLUP);
                    }
                    else
                    {
                        gpio_config_dut_pin_group(dut_pin_group_id, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLDOWN);
                    }
                }
                else
                {
                    gpio_config_dut_pin_group(dut_pin_group_id, DUT_PIN_GROUP_STATE_INPUT, GPIO_NOPULL);
                }

                pin_readback |= (gpio_read_dut_io_pin(dut_pin_group_id) << dut_pin_query_bit);
                pin_readback |= (gpio_read_dut_dir_pin(dut_pin_group_id) << (dut_pin_query_bit + 8));
            }
        }

        *response++ = DAP_OK;
        *response++ = ((pin_readback >> 8) & 0xFF); //Get MSByte (DIRECTION)
        *response++ = (pin_readback & 0xFF);        //Get LSByte (PIN_VALUES)

        num += (3U << 16) | 3U; // 3 bytes read, 2 byte written and 1 byte DAP_OK
        break;
    }
    case ID_DAP_VendorEx35_DUT_PWR_CTRL: {
        //  DUT Power control
        //
        //  This command controls several DUT power signals.  The command has the following bytes:
        //    OUTPUT_VALUE, MASK (optional), DURATION(future)
        //  Note: There is no DIRECTION byte, as these pins are "output only"
        //
        //  OUTPUT_VALUE byte:
        //    bit 0 : UDC_DUT_USB_EN (1 = enabled)
        //    bit 1 : UDC_EXT_RELAY (1 = enabled)
        //    bit 2 : bits 2-7 RSVD
        //
        //  MASK byte:
        //    bit 0 : UDC_DUT_USB_EN
        //    bit 1 : UDC_EXT_RELAY
        //    bit 2 : bits 2-7 RSVD
        //
        //  This command also reads the values from all the gpios (even if writing is masked)
        //  It returns 1 btye, with the PIN_VALUEs
        //  NOTE: If you just want to READ the pins, this command can be used with 0x00, 0x00, 0x00 (to mask writing to any pins)
        uint8_t pins = *request++;
        uint8_t mask = *request;

        uint8_t readback_byte = 0;

        if (mask & 0x1)
        {
            if (pins & 0x1)
            {
                HAL_GPIO_WritePin(UDC_DUT_USB_EN_L_PORT, UDC_DUT_USB_EN_L_PIN, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(UDC_DUT_USB_EN_L_PORT, UDC_DUT_USB_EN_L_PIN, GPIO_PIN_SET);
            }
        }

        if (mask & 0x2)
        {
            if (pins & 0x2)
            {
                HAL_GPIO_WritePin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN, GPIO_PIN_RESET);
            }
        }

        //Readback actual values (even if write is masked out)
        readback_byte |= HAL_GPIO_ReadPin(UDC_DUT_USB_EN_L_PORT, UDC_DUT_USB_EN_L_PIN);
        readback_byte |= (HAL_GPIO_ReadPin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN) << 1);

        *response++ = DAP_OK;
        *response++ = readback_byte;

        num += (2U << 16) | 2U; // 2 bytes read, 1 byte written and 1 byte DAP_OK
        break;
    }
    case ID_DAP_VendorEx36_INTERFACE_VERSION_DETAILS: {
        // Add a more specific internal version string.
        *response++ = DAP_OK;
        num += 1;
        // -2 because we have already filled 2 bytes of the response with Command ID and DAP_OK
        num += udb_get_interface_version(response, DAP_PACKET_SIZE - 2);
        break;
    }
    case ID_DAP_VendorEx37_HOLD_IN_BL:
    {
        // Request to stay in bootloader mode on the next boot for SWU
        *response = DAP_OK;
        config_ram_set_hold_in_bl(true);
        num += 1;
        break;
    }
    case ID_DAP_VendorEx38_RESET_DAPLINK:
    {
        // Request to reset the UDB.
        *response = DAP_OK;
        udb_reset_async(UDB_RESET_TIMER_MS);
        num += 1;
        break;
    }
    case ID_DAP_VendorEx39_READ_UDC_ADAPTER_TYPE_ADC:
    {
        *response++ = DAP_OK;
        udb_adapter_type_t adapter = udb_adapter_detector_get_adapter_type_adc();
        memcpy(response, &adapter, sizeof(adapter));
        num += sizeof(adapter) + 1;
        break;
    }
    case ID_DAP_VendorEx40_MEASURE_POWER:
    {
        num += DAP_ProcessVendorCommandEx40_MeasurePower(request, response);
        break;
    }
    case ID_DAP_VendorEx41_BOOTLOADER_VERSION_DETAILS: {
        // Add a more specific internal version string.
        *response++ = DAP_OK;
        num += 1;
        // -2 because we have already filled 2 bytes of the response with Command ID and DAP_OK
        num += udb_get_bootloader_version(response, DAP_PACKET_SIZE - 2);
        break;
    }
    case ID_DAP_VendorEx42_CHANGE_SWD_DUT:
    {
        uint8_t swd_dut = *request++;
        if (swd_dut < SWD_DUT_COUNT)
        {
            *response++ = DAP_OK;
            change_swd_dut(swd_dut);
        }
        else
        {
            *response++ = DAP_ERROR;
        }
        num += (1U << 16) | 1U;
        break;
    }
    default:
      break;
  }

  return (num);
}
