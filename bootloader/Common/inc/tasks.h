/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* RTL no timeout value */
#define NO_TIMEOUT  (0xffff)

/* Task configuration (Priority, stacks etc.) */
#define LOWEST_PRIORITY             (1)     /* Priority 0 is reserved for the RTX idle task */
#define HIGHEST_PRIORITY            (254)   /* Priority 255 is reserved by RTX */

/* main.c */
#define MSC_TASK_PRIORITY               (5)
#define FLASH_PROGRAMMING_TASK_PRIORITY (10)
#define LED_TASK_PRIORITY (15)

#define MSC_TASK_STACK (200)
