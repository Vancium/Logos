#include "Log.h"
#include "defines.h"

// TODO Temp, move to PCH

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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

static struct tm* get_time_info() {
    time_t rawTime;
    struct tm* timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);
    return timeInfo;
    
}

static void format_time(char* out, struct tm* timeInfo) {
    sprintf(out, " [%d/%d/%d %d:%d:%d]: ", timeInfo->tm_mday,
            timeInfo->tm_mon + 1, timeInfo->tm_year + 1900,
            timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
}

//Outputs log message to stdout
void log_output(log_level level, const char* message, ...) {
    const char* log_level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    const char* log_level_colors[6] = {"\033[1;31m", "\033[1;31m", "\033[1;33m", "\033[1;37m", "\033[1;36m", "\033[1;32m"};
    const char* color_escape_code = "\033[0m";

    b8 is_error = level < 2; // Only true when log_level is FATAL or ERROR

    u16 MSG_LENGTH = 32000;
    u16 TIME_LENGTH = 1024;
    //Buffer for output, 32K is max length for log message, initialize to 0
    char output_message[MSG_LENGTH];
    memset(output_message, 0, sizeof(output_message));

    //Formats variatic arguments into a single string
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(output_message, MSG_LENGTH, message, arg_ptr);
    va_end(arg_ptr);

    //Get time of log
    char time[TIME_LENGTH];
    struct tm* currTimeInfo = get_time_info();
    format_time(time, currTimeInfo);
    


    //Prepend log_level_string to output_message
    char out_msg2[MSG_LENGTH];
    sprintf(out_msg2, "%s%s%s\n", log_level_strings[level], time, output_message);

    printf("%s", log_level_colors[level]);
    printf("%s", out_msg2);
    printf("%s", color_escape_code);


}

