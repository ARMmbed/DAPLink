/*******************************************************************************
* @file    read_uid.c
* @brief   Read unique id from PSoC5LP
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company) 
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/

#include "read_uid.h"
#include "cydevice.h"

void read_unique_id(uint32_t *id)
{

    id[0] = *(uint32_t*) (CYDEV_FLSHID_CUST_TABLES_BASE);
    id[1] = *(uint32_t*) (CYDEV_FLSHID_CUST_TABLES_BASE + 1u);
    id[3] = 0;
    id[4] = 0;
}
