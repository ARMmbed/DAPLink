#include <stdio.h>
#include "shell_cmd_reset.h"
#include "udb_reset.h"

void cmd_reset(int argc, char *argv[])
{
    udb_reset();
}
