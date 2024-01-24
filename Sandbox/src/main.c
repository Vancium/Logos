#include <platform/platform.h>
#include <core/Log/Log.h>

int main(void) {

   LFATAL("Fatal: %f", 3.14); 
   LERROR("Error: %f", 3.14); 
   LWARN("Warn: %f", 3.14); 
   LDEBUG("Debug: %f", 3.14); 
   LINFO("Info: %f", 3.14); 
   LTRACE("Trace: %f", 3.14); 

    platform_state state;

    if (platform_create(&state, "Logos Engine Testbed", 100, 100, 1280, 720)) {
        while(TRUE) {
           platform_pump_messages(&state); 
        }
    }

    platform_destroy(&state);
    
     return 0;
}
