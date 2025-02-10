#include "config.h"

void vApplicationIdleHook(void)
{
    fwdgt_counter_reload();
}
