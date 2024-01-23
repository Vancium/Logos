




#XENON ENGINE SOURCES
set(LIB_SOURCES 
    src/core/Log/Log.c
    src/platform/platform_win32.c
)

set(CMAKE_C_STANDARD 11)

add_library(LogosEngine SHARED ${LIB_SOURCES})




if (UNIX)
    message("UNIX")
    # x11
    find_package(X11 REQUIRED)

    #vulkan
    find_package(Vulkan REQUIRED)

    
    # Set Linker flags
    set(LINK_OPTIONS "-lvulkan;-lxcb;-lX11;-lX11-xcb;-lxkbcommon")
    set_source_files_properties(${LIB_SOURCES} PROPERTIES COMPILE_OPTIONS "-g;-shared;-fdeclspec;-fPIC")
    target_link_libraries(LogosEngine ${X11_LIBRARIES} Vulkan::Vulkan)
    target_include_directories(LogosEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src ${X11_INCLUDE_DIRS})
    add_definitions(-D_DEBUG -DLEXPORT)
endif(UNIX)

if (WIN32)
    message("Windows")

    set(INCLUDE_DIRS 
        src
        src/core
        src/core/Assert 
        src/core/Log
        src/platform
    )
    find_package(Vulkan REQUIRED)
    set(LINK_OPTIONS "-luser32;-lvulkan-1;")
    set_source_files_properties(${LIB_SOURCES} PROPERTIES COMPILE_OPTIONS "-g;-Wvarargs;-Wall;-Werror")
    target_link_libraries(LogosEngine Vulkan::Vulkan)
    target_include_directories(LogosEngine PUBLIC ${INCLUDE_DIRS})
    add_definitions(-D_DEBUG -DLEXPORT -D_CRT_SECURE_NO_WARNINGS)


endif(WIN32)











