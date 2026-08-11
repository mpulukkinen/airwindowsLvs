cmake_minimum_required(VERSION 3.22)

project(LVSEssentialPlugins VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LVS_BUILD_EQ "Build LVS Equalizer" ON)
option(LVS_BUILD_COMPRESSOR "Build LVS Compressor" ON)
option(LVS_BUILD_DELAY "Build LVS Delay" ON)
option(LVS_BUILD_REVERB "Build LVS Reverb" ON)
option(LVS_BUILD_LIMITER "Build LVS Limiter" ON)

# Point this at a local checkout of Steinberg's MIT-licensed VST3 SDK.
# Keeping it external avoids mixing the SDK into the Airwindows source tree.
set(VST3_SDK_ROOT "" CACHE PATH "Path to Steinberg VST3 SDK")

if(NOT VST3_SDK_ROOT)
    message(FATAL_ERROR "Set VST3_SDK_ROOT to a VST3 SDK checkout")
endif()

add_subdirectory("${VST3_SDK_ROOT}" "${CMAKE_BINARY_DIR}/vst3sdk")

add_library(lvs_plugin_common STATIC
    common/LvsAudioProcessor.cpp
)

target_include_directories(lvs_plugin_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)

target_compile_features(lvs_plugin_common PUBLIC cxx_std_20)

include(cmake/LvsAddPlugin.cmake)

if(LVS_BUILD_EQ)
    add_subdirectory(plugins/Equalizer)
endif()

if(LVS_BUILD_COMPRESSOR)
    add_subdirectory(plugins/Compressor)
endif()

if(LVS_BUILD_DELAY)
    add_subdirectory(plugins/Delay)
endif()

if(LVS_BUILD_REVERB)
    add_subdirectory(plugins/Reverb)
endif()

if(LVS_BUILD_LIMITER)
    add_subdirectory(plugins/Limiter)
endif()
