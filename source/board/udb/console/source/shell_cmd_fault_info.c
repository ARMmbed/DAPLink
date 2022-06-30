#include "shell_cmd_fault_info.h"

#include <stdio.h>
#include <string.h>
#include "udb_fault_info.h"

void cmd_fault_info(int argc, char *argv[])
{
    if (strcmp(argv[1], "print") == 0)
    {
        if (udb_is_fault_info_uncleared())
        {
            udb_print_fault_info();
        }
        else
        {
            printf("There's no saved fault info\n");
        }
    }
    else if (strcmp(argv[1], "clear") == 0)
    {
        udb_clear_fault_info();
    }
    else
    {
        printf("Error: wrong args\n");
    }
}
