#include <stdio.h>
#include "core/Log/Log.h"

int main(void) {
    
    initalize_logging();
    LFATAL("Fatal");
    LERROR("Error");
    LWARN("Warn");
    LDEBUG("Debug");
    LINFO("Info");
    LTRACE("Trace");
    return 0;
}
