
#include "core/Log/Log.h"

#include "platform/platform.h"

int main(void) {

    LFATAL("FATAL");
    LERROR("ERROR")
    LWARN("WARN");
    LINFO("INFO");
    LDEBUG("DEBUG");
    LTRACE("TRACE");
    

    platform_state state;

    if (platform_create(&state, "Logos Engine Testbed", 100, 100, 1280, 720)) {
        while(TRUE) {
           platform_pump_messages(&state); 
        }
    }

    platform_destroy(&state);
    
     return 0;
}
