#include "platform.h"
#include <bits/time.h>
#include <xcb/xproto.h>

// Platform specific state, contains window handle, platform specific functions etc
// type is detemened in specific platform implementation


#if LPLATFORM_LINUX

#include "../core/Log/Log.h"
#include "../defines.h"
#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <sys/time.h>
#include <X11/Xlib-xcb.h>

#if _POSIX_C_SOURCE >=  199309L
#include <time.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct linux_layer_state {
    Display* display;
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
} linux_platform_state;

LAPI b8 platform_create(
    platform_state* state,
    const char* name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
) {
    // Internal Linux state
    state->internal_state = malloc(sizeof(linux_platform_state));
    linux_platform_state* curr_state = (linux_platform_state*)state->internal_state;

    //Connect to X server
    curr_state->display = XOpenDisplay(NULL);

    // Turn off Keyrepeats
    XAutoRepeatOff(curr_state->display);


    curr_state->connection = XGetXCBConnection(curr_state->display);

    if (xcb_connection_has_error(curr_state->connection)) {
        LFATAL("Failed to connect to X server via XCB");
        return FALSE;
    }

    // Get data from X server
    const struct xcb_setup_t* setup = xcb_get_setup(curr_state->connection);

    //iterate through screens
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(setup);
    int screen_p = 0;
    for (i32 s = screen_p; s > 0; s--) {
        xcb_screen_next(&screen_iter);
    }

    // assign screen data
    curr_state->screen = screen_iter.data;

    //Allocate XID for window creation
    curr_state->window = xcb_generate_id(curr_state->connection);


    //Register Event Types
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    /*
    *   Listen for these event values
    *
    *   Button Press
    *   Button Release
    *   Key Press
    *   Key Release
    *   Exposure
    *   Pointer Motion
    *   Structure Notify
    */
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | 
                        XCB_EVENT_MASK_BUTTON_RELEASE | 
                        XCB_EVENT_MASK_KEY_PRESS | 
                        XCB_EVENT_MASK_KEY_RELEASE | 
                        XCB_EVENT_MASK_EXPOSURE |
                        XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    u32 value_list[] = {curr_state->screen->black_pixel, event_values};

    //Create window
    xcb_void_cookie_t cookie = xcb_create_window(
        curr_state->connection,
        XCB_COPY_FROM_PARENT, // Depth
        curr_state->window,
        curr_state->screen->root, // Parent
        x,
        y,
        width,
        height,
        0, // No border
        XCB_WINDOW_CLASS_INPUT_OUTPUT, // Class
        curr_state->screen->root_visual,
        event_mask,
        value_list
    );

    //Set Window title
    xcb_change_property(
        curr_state->connection,
        XCB_PROP_MODE_REPLACE,
        curr_state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8, // Data should be view 8 bits at a time
        strlen(name),
        name
    );

    //Tells server to notify window manager
    //attempts to destroy the window
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(
        curr_state->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW"
    );
  
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(
        curr_state->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );
    
    xcb_intern_atom_reply_t* wm_delete_reply = xcb_intern_atom_reply(
        curr_state->connection,
        wm_delete_cookie,
        NULL
    );
    xcb_intern_atom_reply_t* wm_protocols_reply = xcb_intern_atom_reply(
        curr_state->connection,
        wm_protocols_cookie,
        NULL
    );
    curr_state->wm_delete_win = wm_delete_reply->atom;
    curr_state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property(
        curr_state->connection,
        XCB_PROP_MODE_REPLACE,
        curr_state->window,
        wm_protocols_reply->atom,
        4,
        32,
        1,
        &wm_delete_reply->atom
    );

    //map window to screen
    xcb_map_window(curr_state->connection, curr_state->window);

    //Flush stream
    i32 stream_result = xcb_flush(curr_state->connection);
    if (stream_result <= 0) {
        LFATAL("Error occured when flushing stream: %d", stream_result);
        return FALSE;
    }


    return TRUE;
}

LAPI void platform_destroy(platform_state* state) {
    // cold cast linux_layer_state
    linux_platform_state* curr_state = (linux_platform_state*)state->internal_state;

    //Turn on repeat keys
    XAutoRepeatOn(curr_state->display);

    xcb_destroy_window(curr_state->connection, curr_state->window);
}

LAPI b8 platform_pump_messages(platform_state* state) {

    // cold cast linux_layer_state
    linux_platform_state* curr_state = (linux_platform_state*)state->internal_state;

    xcb_generic_event_t* event;
    xcb_client_message_event_t* cm;

    b8 quit_flagged = FALSE;

    while(event != 0) {
        event = xcb_poll_for_event(curr_state->connection);
        if (event == 0) {
            break;
        }

        //Why xcb??
        switch(event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE: {
                //TODO Handle Key press & release
            } break;
            case XCB_BUTTON_PRESS:
            case XCB_BUTTON_RELEASE: {
                //TODO Handle mouse button press & release
            } break;
            case XCB_MOTION_NOTIFY: {
                //TODO Handle mouse movement
            } break;
            case XCB_CONFIGURE_NOTIFY: {
                // TODO Handle Resizing
            } break;
            case XCB_CLIENT_MESSAGE: {
                cm = (xcb_client_message_event_t*)event;

                if(cm->data.data32[0] == curr_state->wm_delete_win) {
                    quit_flagged = TRUE;
                }
            } break;
            default: {
                //TODO do something
            } break;
        }
         free(event);

    }
    return !quit_flagged;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}
void platform_free(void* block, b8 aligned) {
    free(block);
}
void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}
void* platform_copy_memory(void* dest, const void* src, u64 size) {
    return memcpy(dest, src, size);
}
void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 color) {
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], message);
}
void platform_console_write_error(const char* message, u8 color) {
    const char* color_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", color_strings[color], message);
}

f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_nsec * now.tv_sec * 0.000000001;
}

void platform_sleep(u64 ms) {
    #if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000 * 1000;
        nanosleep(&ts, 0);
    #else
        if (ms >= 1000) {
            sleep(ms / 1000);
        }
        usleep((ms % 1000) * 1000);
    #endif


}

#endif
