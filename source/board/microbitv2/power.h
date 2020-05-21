#ifndef POWER_H_
#define POWER_H_

#include "fsl_common.h"
#include "fsl_smc.h"

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 0,
    kAPP_PowerModeRun,   /* Normal RUN mode */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeVlls1, /* VLLS1 mode. */
    kAPP_PowerModeVlls3, /* VLLS3 mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

void power_init(void);

/* Lowest power mode available in KL27*/
void power_enter_VLLS0(void);

/* Lowest power mode that allows I2C operation with address match wakeup */
void power_enter_VLPS(void);

#endif /* POWER_H_ */
