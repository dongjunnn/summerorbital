cmake_minimum_required(VERSION 3.20)
project(summerorbital LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- SDL2 family ---
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

# --- ENet discovery (as you had) ---
find_package(ENet CONFIG QUIET)
find_package(enet CONFIG QUIET)
if(NOT TARGET ENet::ENet)
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(ENET QUIET IMPORTED_TARGET enet)
    if(TARGET PkgConfig::ENET)
      add_library(ENet::ENet ALIAS PkgConfig::ENET)
    else()
      pkg_check_modules(LIBENET QUIET IMPORTED_TARGET libenet)
      if(TARGET PkgConfig::LIBENET)
        add_library(ENet::ENet ALIAS PkgConfig::LIBENET)
      endif()
    endif()
  endif()
endif()
if(NOT TARGET ENet::ENet)
  find_path(ENET_INCLUDE_DIR enet/enet.h
    HINTS "C:/msys64/mingw64/include" $ENV{CMAKE_PREFIX_PATH})
  find_library(ENET_LIBRARY NAMES enet libenet
    HINTS "C:/msys64/mingw64/lib" $ENV{CMAKE_PREFIX_PATH})
  if(ENET_INCLUDE_DIR AND ENET_LIBRARY)
    add_library(ENet::ENet UNKNOWN IMPORTED)
    set_target_properties(ENet::ENet PROPERTIES
      IMPORTED_LOCATION "${ENET_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ENET_INCLUDE_DIR}")
  else()
    message(FATAL_ERROR "ENet not found. Install mingw-w64-x86_64-enet or set enet_DIR / CMAKE_PREFIX_PATH.")
  endif()
endif()

# --- Engine library ---
file(GLOB_RECURSE ASTROENGINE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/AstroEngine/*.[ch]pp"
  "${CMAKE_SOURCE_DIR}/AstroEngine/*.c"
)
add_library(astro_engine STATIC ${ASTROENGINE_SOURCES})
target_include_directories(astro_engine PUBLIC "${CMAKE_SOURCE_DIR}/AstroEngine")

# IMPORTANT: do NOT link SDL2main; define SDL_MAIN_HANDLED instead
target_compile_definitions(astro_engine PUBLIC SDL_MAIN_HANDLED)

target_link_libraries(astro_engine
  PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    ENet::ENet
)

# --- Copy assets next to built binaries ---
function(copy_assets tgt)
  if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${tgt} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
              "${CMAKE_SOURCE_DIR}/assets"
              "$<TARGET_FILE_DIR:${tgt}>/assets")
  endif()
endfunction()

# --- One executable ---
add_executable(summerorbital_app
  AstroEngine/Src/main.cpp
)
# also define here, just in case main.cpp includes SDL before engine headers
target_compile_definitions(summerorbital_app PRIVATE SDL_MAIN_HANDLED)
target_link_libraries(summerorbital_app PRIVATE astro_engine)
copy_assets(summerorbital_app)

# Warnings (optional)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
