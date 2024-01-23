#include "platform.h"

// Windows platform layer

#if LPLATFORM_WINDOWS

#include "../core/Log/Log.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

typedef struct win32_layer_state {
    HINSTANCE h_instance;
    HWND h_windowHandle;
} win32_layer_state;

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

b8 platform_create(platform_state *state, const char *name, i32 x, i32 y, i32 width, i32 height) {
    //Allocate memory for win32 state
    state->internal_state = malloc(sizeof(win32_layer_state));
    //
    win32_layer_state* current_state = (win32_layer_state*)state->internal_state;

    current_state->h_instance = GetModuleHandleA(0); // Gets handle of application currently being executed
    

    HICON icon = LoadIcon(current_state->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;


    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = current_state->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "logos_window_class";

    if(!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    //Create window
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRect(&border_rect, window_style, window_ex_style);

    window_x += border_rect.left;
    window_y += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(window_ex_style, "logos_window_class", name, window_style, window_x, window_y, window_width, window_height, 0, 0, current_state->h_instance, 0);

    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed", "Error!", MB_ICONEXCLAMATION | MB_OK);
        LFATAL("Window creation failed!");
        return FALSE;
        
    } else {
        current_state->h_windowHandle = handle;
    }

    b32 should_activate = 1;

    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(current_state->h_windowHandle, show_window_command_flags);

    //Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}


void platform_destroy(platform_state* state) {
    win32_layer_state* curr_state = (win32_layer_state*)state->internal_state;

    if (curr_state->h_windowHandle) {
        DestroyWindow(curr_state->h_windowHandle);
        curr_state->h_windowHandle = 0;
    }
}

b8 platform_pump_messages(platform_state *state) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return TRUE;
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

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);

}
void platform_console_write_error(const char* message, u8 color) {
   HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return (f64)current_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}



LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch(msg) {
        case WM_ERASEBKGND: {
            //Notify OS that application will handle erasing of window
            return 1;
        }
        case WM_CLOSE: {
            // TODO fire event for application to quit
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE: {
            //RECT r;
            //GetClientRect(hwnd, &r);
            //u32 width = r.right - r.left;
            //u32 height = r.bottom - r.top;
            //TODO
            //Fire event for window resize

        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            //b8 pressed = (msg == WM_KEYDOWN || WM_SYSKEYDOWN);
            //TODO fire event for keydown


        } break;
        case WM_MOUSEMOVE: {
            //i32 x_pos = GET_X_LPARAM(l_param);
            //i32 y_pos = GET_Y_LPARAM(l_param);
            //TODO process input
        } break;
        case WM_MOUSEWHEEL: {
            //i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            //if (z_delta != 0) {
                //Flatten to os independent (-1, 1) up or down
                //z_delta = (z_delta < 0) ? -1 : 1;
            //}
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            // b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
            //TODO input processing

        } break;
    }
    return DefWindowProcA(hwnd, msg, w_param, l_param);
}


#endif
