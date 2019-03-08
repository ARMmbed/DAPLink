/*******************************************************************************
* File Name: Timer_CSTick_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
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

#include "Timer_CSTick.h"

static Timer_CSTick_backupStruct Timer_CSTick_backup;


/*******************************************************************************
* Function Name: Timer_CSTick_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_CSTick_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_CSTick_SaveConfig(void)
{
    #if (!Timer_CSTick_UsingFixedFunction)
        Timer_CSTick_backup.TimerUdb = Timer_CSTick_ReadCounter();
        Timer_CSTick_backup.InterruptMaskValue = Timer_CSTick_STATUS_MASK;
        #if (Timer_CSTick_UsingHWCaptureCounter)
            Timer_CSTick_backup.TimerCaptureCounter = Timer_CSTick_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_CSTick_UDB_CONTROL_REG_REMOVED)
            Timer_CSTick_backup.TimerControlRegister = Timer_CSTick_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_CSTick_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_CSTick_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_CSTick_RestoreConfig(void)
{
    #if (!Timer_CSTick_UsingFixedFunction)

        Timer_CSTick_WriteCounter(Timer_CSTick_backup.TimerUdb);
        Timer_CSTick_STATUS_MASK =Timer_CSTick_backup.InterruptMaskValue;
        #if (Timer_CSTick_UsingHWCaptureCounter)
            Timer_CSTick_SetCaptureCount(Timer_CSTick_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_CSTick_UDB_CONTROL_REG_REMOVED)
            Timer_CSTick_WriteControlRegister(Timer_CSTick_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_CSTick_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_CSTick_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_CSTick_Sleep(void)
{
    #if(!Timer_CSTick_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_CSTick_CTRL_ENABLE == (Timer_CSTick_CONTROL & Timer_CSTick_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_CSTick_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_CSTick_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_CSTick_Stop();
    Timer_CSTick_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_CSTick_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Timer_CSTick_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_CSTick_Wakeup(void)
{
    Timer_CSTick_RestoreConfig();
    #if(!Timer_CSTick_UDB_CONTROL_REG_REMOVED)
        if(Timer_CSTick_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_CSTick_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
