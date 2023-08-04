#include "shell_cmd_uptime.h"
#include <string.h>
#include <stdio.h>

#include "udb_uptime.h"

void cmd_uptime(int argc, char *argv[])
{
    udb_uptime_print();
}
