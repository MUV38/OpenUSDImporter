cmake_minimum_required(VERSION 3.24)

project(OpenUSDImporter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OpenUSD_RELEASE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/OpenUSD-v26.05-install" CACHE PATH "Release OpenUSD install directory")
set(OpenUSD_DEBUG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/OpenUSD-v26.05-debug-install" CACHE PATH "Debug OpenUSD install directory")

if(NOT EXISTS "${OpenUSD_RELEASE_ROOT}/include/pxr/pxr.h")
    message(FATAL_ERROR "Release OpenUSD was not found: ${OpenUSD_RELEASE_ROOT}")
endif()

if(NOT EXISTS "${OpenUSD_DEBUG_ROOT}/include/pxr/pxr.h")
    message(FATAL_ERROR "Debug OpenUSD was not found: ${OpenUSD_DEBUG_ROOT}")
endif()

add_executable(OpenUSDImporter
    src/main.cpp
)

target_include_directories(OpenUSDImporter PRIVATE
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/include>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/include>"
)

target_compile_definitions(OpenUSDImporter PRIVATE
    OPENUSD_IMPORTER_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(OpenUSDImporter PRIVATE
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/usd_ms.lib>"
    "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/tbb_debug.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/usd_ms.lib>"
    "$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/tbb.lib>"
    Ws2_32.lib
    Dbghelp.lib
    Shlwapi.lib
)

if(WIN32)
    target_compile_definitions(OpenUSDImporter PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        TBB_SUPPRESS_DEPRECATED_MESSAGES=1
    )
    target_compile_options(OpenUSDImporter PRIVATE /utf-8 /external:W0 /wd4273)
    set_property(TARGET OpenUSDImporter PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    set_property(TARGET OpenUSDImporter PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "assets/CubeMesh.usda")
    set_property(TARGET OpenUSDImporter PROPERTY
        VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/bin>$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/bin>;%PATH%"
    )

    add_custom_command(TARGET OpenUSDImporter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/bin>$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/bin>"
                "$<TARGET_FILE_DIR:OpenUSDImporter>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/usd_ms.dll>$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/usd_ms.dll>"
                "$<TARGET_FILE_DIR:OpenUSDImporter>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "$<$<CONFIG:Debug>:${OpenUSD_DEBUG_ROOT}/lib/usd>$<$<NOT:$<CONFIG:Debug>>:${OpenUSD_RELEASE_ROOT}/lib/usd>"
                "$<TARGET_FILE_DIR:OpenUSDImporter>/usd"
        COMMENT "Copying OpenUSD runtime files"
    )
endif()
