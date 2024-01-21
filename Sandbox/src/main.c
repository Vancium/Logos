#include <stdio.h>
#include "core/Assert/Asserts.h"
#include "core/Log/Log.h"

int main(void) {
    
    initalize_logging();
    LFATAL("Fatal: %f", 3.14);
    LERROR("Error: %f", 3.14);
    LWARN("Warn: %f", 3.14);
    LDEBUG("Debug: %f", 3.14);
    LINFO("Info: %f", 3.14);
    LTRACE("Trace: %f", 3.14);
    return 0;
}
