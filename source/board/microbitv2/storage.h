/**
 * @file    storage.h
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2020 NXP
 * Copyright 2021 Micro:bit Educational foundation
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
 
#ifndef STORAGE_H_
#define STORAGE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t storage_program_page(uint32_t adr, uint32_t sz, uint8_t *buf);
uint32_t storage_erase_sector(uint32_t adr);
uint32_t storage_erase_all(void);

#ifdef __cplusplus
}
#endif

#endif /* STORAGE_H_ */
