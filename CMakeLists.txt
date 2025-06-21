cmake_minimum_required(VERSION 3.10)
project(AstroEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# — SDL2 via CMake’s built-in FindSDL2 —
find_package(SDL2 REQUIRED)
message(STATUS "Found SDL2: ${SDL2_LIBRARIES}")

# — SDL2_image via pkg-config (requires libsdl2-image-dev + pkg-config) —
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
message(STATUS "Found SDL2_image: ${SDL2_IMAGE_LIBRARIES}")

# ENet as a subdirectory
add_subdirectory(enet-1.3.18)

# Include directories
include_directories(
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/enet-1.3.18/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/Components"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/ECS"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/ECS/Net"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/GameStates/ClientStates"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/GameStates/ServerStates"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/GameStates/Shared"
  "${CMAKE_CURRENT_SOURCE_DIR}/AstroEngine/Src/Systems"
)

# Gather sources
file(GLOB_RECURSE SOURCES "AstroEngine/Src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  enet
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
)

# Platform-specific extras
if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 winmm)
endif()

message(STATUS "Configuration complete—run ‘make’ now.") 
