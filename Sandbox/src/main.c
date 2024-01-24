#include <core/Log/Log.h>
#include <core/Application/Application.h>

int main(void) {

    application_config config;
    config.start_y_pos = 100;
    config.start_x_pos = 100;
    config.start_width = 1280;
    config.start_height = 720;
    config.name = "Logos Engine Sandbox";

    application_create(&config);
    application_run();
    
    return 0;
}
