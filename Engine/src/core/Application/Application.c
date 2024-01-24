#include "Application.h"

#include "../../core/Log/Log.h"
#include "../../platform/platform.h"

typedef struct application_state {
    b8 is_running;
    b8 is_suspended;
   platform_state plat_state;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;


b8 application_create(application_config* config) {

    if(initialized) {
        LERROR("application_create called more than once");
        return FALSE;
    }


    //Initialize subsystems
    initalize_logging();
    LFATAL("Fatal: %f", 3.12f);
    LERROR("Error: %f", 3.12f);
    LWARN("Warn: %f", 3.12f);
    LINFO("Info: %f", 3.12f);
    LDEBUG("Debug: %f", 3.12f);
    LTRACE("Trace: %f", 3.12f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    platform_state state;

    if(!platform_create(
        &app_state.plat_state, 
        config->name, 
        config->start_x_pos, 
        config->start_y_pos, 
        config->start_width, 
        config->start_height)) {
        return FALSE;
    }
    



    initialized = TRUE;

    return TRUE;

}
b8 application_run() {
    while(app_state.is_running) {
        if(!platform_pump_messages(&app_state.plat_state)) {
            app_state.is_running = FALSE;
        }
    }

    app_state.is_running = FALSE;
    platform_destroy(&app_state.plat_state);

    return TRUE;
};
