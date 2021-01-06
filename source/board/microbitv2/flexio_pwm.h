/**
 * @file    flexio_pwm.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * Copyright 2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
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
