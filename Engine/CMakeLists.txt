




#XENON ENGINE SOURCES
set(LIB_SOURCES 
    src/core/Log/Log.c

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
    set_source_files_properties(%{LIB_SOURCES} PROPERTIES COMPILE_OPTIONS "-g;-shared;-fdeclspec;-fPIC")
    target_link_libraries(LogosEngine ${X11_LIBRARIES} Vulkan::Vulkan)
endif(UNIX)



add_definitions(-D_DEBUG -DLEXPORT)

target_include_directories(LogosEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src ${X11_INCLUDE_DIRS})







