cmake_minimum_required(VERSION 3.16)
project(SummerOrbital LANGUAGES C CXX)

# Engine lib (optional). If you don't have one, remove this block.
file(GLOB_RECURSE ENGINE_SRC Src/*.cpp Src/*.h)
add_library(astro_engine STATIC ${ENGINE_SRC})

add_executable(summerorbital_app AstroEngine/Src/main.cpp)
target_link_libraries(summerorbital_app PRIVATE astro_engine)

# Normal 'int main(...)'
target_compile_definitions(summerorbital_app PRIVATE SDL_MAIN_HANDLED)

# vcpkg manifest mode puts headers/libs here:
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET x64-mingw-static)
endif()
set(VCPKG_DIR     "${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}")
set(VCPKG_LIB_DIR "${VCPKG_DIR}/lib")
set(VCPKG_INC_DIR "${VCPKG_DIR}/include")

# Make the vcpkg headers/libs visible
target_include_directories(summerorbital_app PRIVATE "${VCPKG_INC_DIR}")
target_link_directories(summerorbital_app PRIVATE "${VCPKG_LIB_DIR}")

# Link by plain library names -> lib*.a in VCPKG_LIB_DIR
target_link_libraries(summerorbital_app PRIVATE
  SDL2
  SDL2main
  SDL2_image
  SDL2_ttf
  enet
  # Windows system libs needed by SDL/ENet
  ws2_32 winmm imm32 version ole32 oleaut32 setupapi shell32 advapi32 gdi32 user32
)

# Optional: install a ready-to-zip folder
include(GNUInstallDirs)
install(TARGETS summerorbital_app RUNTIME DESTINATION .)
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
  install(DIRECTORY assets/ DESTINATION assets)
endif()
