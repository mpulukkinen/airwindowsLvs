cmake_minimum_required(VERSION 3.22)

project(LVSEssentialPlugins VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Stage 1 builds the reusable DSP layer without requiring any plugin SDK.
# Turn this on once the first VST3 wrapper target is added.
option(LVS_ENABLE_VST3 "Build LVS VST3 plugin targets" OFF)

add_library(lvs_plugin_common STATIC
    common/LvsAudioProcessor.cpp
    dsp/PearLiteEqDsp.cpp
)

target_include_directories(lvs_plugin_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)

target_compile_features(lvs_plugin_common PUBLIC cxx_std_20)

if(LVS_ENABLE_VST3)
    set(VST3_SDK_ROOT "" CACHE PATH "Path to Steinberg VST3 SDK")

    if(NOT VST3_SDK_ROOT)
        message(FATAL_ERROR "Set VST3_SDK_ROOT to a VST3 SDK checkout")
    endif()

    add_subdirectory("${VST3_SDK_ROOT}" "${CMAKE_BINARY_DIR}/vst3sdk")

    # Individual plugin targets will be added here as their processor/controller
    # and native editor implementations land.
endif()
