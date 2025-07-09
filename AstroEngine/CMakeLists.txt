file(GLOB_RECURSE SRC_FILES
    Src/*.cpp
    Src/*.h
)

add_executable(AstroEngine ${SRC_FILES})

include_directories(/usr/include/SDL2)
link_directories(/usr/lib/x86_64-linux-gnu)

target_link_libraries(AstroEngine
    SDL2
    SDL2_image
    SDL2_ttf
    enet
)
