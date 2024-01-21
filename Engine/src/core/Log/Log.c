#include "Log.h"
#include "defines.h"

// TODO Temp, move to PCH

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

b8 initalize_logging() {
    // TODO Create log file
    return TRUE;
}
void shutdown_logging() {
    //TODO Clean up log files, write out queued entries
}

//Outputs log message to stdout
LAPI void log_output(log_level level, const char* message, ...) {
    const char* log_level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    const char* log_level_colors[6] = {"\033[1;31m", "\033[1;31m", "\033[1;33m", "\033[1;37,", "\033[1;36m", "\033[1;32m"};
    const char* color_escape_code = "\033[0m";

    b8 is_error = level < 2; // Only true when log_level is FATAL or ERROR

    u16 MSG_LENGTH = 32000;
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

    printf("%s", log_level_colors[level]);
    printf("%s", out_msg2);
    printf("%s", color_escape_code);



    
    
}
