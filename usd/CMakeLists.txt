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

add_library(usd STATIC
    src/usd.cpp
)

if(CMAKE_VS_PLATFORM_NAME)
    set(USD_OUTPUT_PLATFORM "${CMAKE_VS_PLATFORM_NAME}")
else()
    set(USD_OUTPUT_PLATFORM "${CMAKE_SYSTEM_PROCESSOR}")
endif()

set_target_properties(usd PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${USD_OUTPUT_PLATFORM}/$<CONFIG>"
)

target_compile_features(usd PUBLIC cxx_std_17)

target_include_directories(usd PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/include>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/include>"
)

target_link_libraries(usd PUBLIC
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/usd_ms.lib>"
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/tbb_debug.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/usd_ms.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/tbb.lib>"
)

target_compile_definitions(usd PUBLIC
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    TBB_SUPPRESS_DEPRECATED_MESSAGES=1
)

target_compile_options(usd PUBLIC
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/wd4273>"
)

add_custom_command(TARGET usd POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${OpenUSD_DEBUG_ROOT},${OpenUSD_RELEASE_ROOT}>/lib/usd_ms.lib"
        "$<TARGET_FILE_DIR:usd>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${OpenUSD_DEBUG_ROOT},${OpenUSD_RELEASE_ROOT}>/lib/$<IF:$<CONFIG:Debug>,tbb_debug.lib,tbb.lib>"
        "$<TARGET_FILE_DIR:usd>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${OpenUSD_DEBUG_ROOT},${OpenUSD_RELEASE_ROOT}>/lib/usd_ms.dll"
        "$<TARGET_FILE_DIR:usd>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "$<IF:$<CONFIG:Debug>,${OpenUSD_DEBUG_ROOT},${OpenUSD_RELEASE_ROOT}>/bin"
        "$<TARGET_FILE_DIR:usd>/bin"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "$<IF:$<CONFIG:Debug>,${OpenUSD_DEBUG_ROOT},${OpenUSD_RELEASE_ROOT}>/lib/usd"
        "$<TARGET_FILE_DIR:usd>/usd"
)
