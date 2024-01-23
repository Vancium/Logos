#include "Log.h"
#include "../../defines.h"
#include "../../platform/platform.h"

// TODO Temp, move to PCH

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "../Assert/Asserts.h"


void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}

b8 initalize_logging() {
    // TODO Create log file
    return TRUE;
}
void shutdown_logging() {
    //TODO Clean up log files, write out queued entries
}




//Outputs log message to stdout
void log_output(log_level level, const char* message, ...) {
    const char* log_level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};

    b8 is_error = level < LOG_LEVEL_WARN; // Only true when log_level is FATAL or ERROR
    //
    if (is_error) {
        printf("IS ERROR");
    }

    
    const u16 MSG_LENGTH = 32000;
    //Buffer for output, 32K is max length for log message, initialize to 0
    char output_message[MSG_LENGTH];
    memset(output_message, 0, sizeof(output_message));

    //Formats variatic arguments into a single string
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(output_message, MSG_LENGTH, message, arg_ptr);
    va_end(arg_ptr);

    


    //Prepend log_level_string to output_message
    char out_msg2[MSG_LENGTH];
    sprintf(out_msg2, "%s%s\n", log_level_strings[level], output_message);


    //Platform specific platform
    if (is_error) {
        platform_console_write_error(out_msg2, level);
    } else {
        platform_console_write(out_msg2, level);
    }


}

