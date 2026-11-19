#include "daplink_swd_dynamic_pins.h"

static swd_pin_set_t latched_configured_pins = SWD_PIN_SET_DEFAULT;
static swd_pin_set_t pending_configured_pins = SWD_PIN_SET_DEFAULT;

swd_pin_set_t swd_configure_pins(swd_pin_set_t pin_set) {
    pending_configured_pins = pin_set;
    return latched_configured_pins;
}

swd_pin_set_t swd_get_configured_pins(void) {
    return latched_configured_pins;
}

void swd_configure_latch_pins(void) {
    latched_configured_pins = pending_configured_pins;
}
