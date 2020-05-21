#ifndef FLEXIO_PWM_H_
#define FLEXIO_PWM_H_

#include "IO_Config.h"

#ifdef __cplusplus
extern "C" {
#endif

void flexio_pwm_init(void);
void flexio_pwm_init_pins(void);
void flexio_pwm_deinit_pins(void);
void flexio_pwm_set_dutycycle(uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif /* FLEXIO_PWM_H_ */
