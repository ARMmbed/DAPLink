#ifndef DAPLINK_SWD_DYNAMIC_PINS_H_
#define DAPLINK_SWD_DYNAMIC_PINS_H_

typedef enum {
    SWD_PIN_SET_DEFAULT,
    SWD_PIN_SET_ALT_1,

    SWD_PIN_SET_MAX_ALLOWED = SWD_PIN_SET_ALT_1,
} swd_pin_set_t;

// Stages the provided `pin_set` for latching on the next target connection.
//
// Returns the currently latched `swd_pin_set_t`.
swd_pin_set_t swd_configure_pins(swd_pin_set_t pin_set);

// Gets the currently latched `swd_pin_set_t`.
swd_pin_set_t swd_get_configured_pins(void);

// Latches the last configured `swd_pin_set_t` choice.
void swd_configure_latch_pins(void);

#endif // DAPLINK_SWD_DYNAMIC_PINS_H_
