#include "swd_host.h"
#include "target_family.h"

static void stm32l475vg_target_before_init_debug(void)
{
    // any target specific sequences needed before attaching
    //	to the DAP across JTAG or SWD
    return;
}

static uint8_t stm32l475vg_target_unlock_sequence(void)
{
    // if the device can secure the flash and there is a way to
    //	erase all it should be implemented here.
    return 1;
}

static uint8_t stm32l475vg_target_set_state(target_state_t state)
{
    // if a custom state machine is needed to set the TARGET_RESET_STATE state
    return 1;
}

static uint8_t stm32l475vg_security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    // if there are security bits in the programmable flash region
    //	a check should be performed. This method is used when programming
    //	by drag-n-drop and should refuse to program an image requesting
    //	to set the device security. This can be performed with the debug channel
    //	if needed.
    return 0;
}

const target_family_descriptor_t stm32l475vg_target_family = {
    .family_id = kStub_HWReset_FamilyID,
    .default_reset_type = kHardwareReset,
    // .target_before_init_debug = stm32l475vg_target_before_init_debug,
    // .target_unlock_sequence = stm32l475vg_target_unlock_sequence,
    // .target_set_state = stm32l475vg_target_set_state,
    // .security_bits_set = stm32l475vg_security_bits_set,
};

const target_family_descriptor_t *g_target_family = &stm32l475vg_target_family;