cmake_minimum_required(VERSION 3.24)

project(usd LANGUAGES CXX)

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

set(OpenUSD_RELEASE_ROOT "${REPO_ROOT}/external/OpenUSD-v26.05-install" CACHE PATH "Release OpenUSD install directory")
set(OpenUSD_DEBUG_ROOT "${REPO_ROOT}/external/OpenUSD-v26.05-debug-install" CACHE PATH "Debug OpenUSD install directory")

if(NOT EXISTS "${OpenUSD_RELEASE_ROOT}/include/pxr/pxr.h")
    message(FATAL_ERROR "Release OpenUSD was not found: ${OpenUSD_RELEASE_ROOT}")
endif()

if(NOT EXISTS "${OpenUSD_DEBUG_ROOT}/include/pxr/pxr.h")
    message(FATAL_ERROR "Debug OpenUSD was not found: ${OpenUSD_DEBUG_ROOT}")
endif()

add_library(usd INTERFACE)

target_include_directories(usd INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/include>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/include>"
)

target_link_libraries(usd INTERFACE
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/usd_ms.lib>"
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/tbb_debug.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/usd_ms.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/tbb.lib>"
)

target_compile_definitions(usd INTERFACE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    TBB_SUPPRESS_DEPRECATED_MESSAGES=1
)
