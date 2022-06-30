#include <stdio.h>
#include "shell_cmd_adapter_type.h"
#include "udb_adapter_detector.h"

void cmd_adapter_type(int argc, char *argv[])
{
    udb_adapter_type_t adapter_type = udb_adapter_detector_get_adapter_type_adc();
    printf("%s\n", udb_adapter_detector_get_adapter_type_name(adapter_type));
}
