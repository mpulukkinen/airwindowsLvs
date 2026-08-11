cmake_minimum_required(VERSION 3.22)

project(LVSEssentialPlugins VERSION 0.1.0 LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "LVS Essential Plugins currently support Windows only")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Avoid requiring administrator rights or developer-mode symlink privileges.
set(SMTG_CREATE_PLUGIN_LINK OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VSTGUI_SUPPORT ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    vst3sdk
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(vst3sdk)
smtg_enable_vst3_sdk()

add_library(lvs_plugin_common STATIC
    common/LvsAudioProcessor.cpp
)
target_include_directories(lvs_plugin_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)
target_compile_features(lvs_plugin_common PUBLIC cxx_std_20)

# Selected original Airwindows VST2 source files are compiled only as private
# DSP implementation. legacy/audioeffectx.h is a minimal compatibility shim;
# no VST2 ABI is shipped or exposed by the resulting LVS VST3 plugins.
set(AIRWINDOWS_WINVST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../plugins/WinVST")
set(AIRWINDOWS_TAPE_DELAY_CPP "${AIRWINDOWS_WINVST_ROOT}/TapeDelay2/TapeDelay2.cpp")
set(AIRWINDOWS_REVERB_CPP "${AIRWINDOWS_WINVST_ROOT}/Reverb/Reverb.cpp")

# Both legacy source files define the generic VST2 createEffectInstance symbol.
# Rename those private, unused entry points so they can coexist in one library.
set_source_files_properties(${AIRWINDOWS_TAPE_DELAY_CPP} PROPERTIES
    COMPILE_DEFINITIONS "createEffectInstance=createTapeDelay2EffectInstance"
)
set_source_files_properties(${AIRWINDOWS_REVERB_CPP} PROPERTIES
    COMPILE_DEFINITIONS "createEffectInstance=createAirwindowsReverbEffectInstance"
)

add_library(lvs_airwindows_legacy STATIC
    ${AIRWINDOWS_TAPE_DELAY_CPP}
    ${AIRWINDOWS_WINVST_ROOT}/TapeDelay2/TapeDelay2Proc.cpp
    ${AIRWINDOWS_REVERB_CPP}
    ${AIRWINDOWS_WINVST_ROOT}/Reverb/ReverbProc.cpp
)
target_include_directories(lvs_airwindows_legacy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/legacy
    ${AIRWINDOWS_WINVST_ROOT}/TapeDelay2
    ${AIRWINDOWS_WINVST_ROOT}/Reverb
)
target_compile_definitions(lvs_airwindows_legacy PRIVATE
    _USE_MATH_DEFINES
    _CRT_SECURE_NO_WARNINGS
    NOMINMAX
)
target_compile_features(lvs_airwindows_legacy PUBLIC cxx_std_20)

add_library(lvs_airwindows_dsp STATIC
    dsp/PearLiteEqDsp.cpp
    dsp/ButterComp2Dsp.cpp
    dsp/TapeDelay2Dsp.cpp
    dsp/AirwindowsReverbDsp.cpp
)
target_include_directories(lvs_airwindows_dsp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/dsp
)
target_link_libraries(lvs_airwindows_dsp PUBLIC
    lvs_plugin_common
    lvs_airwindows_legacy
)
target_compile_features(lvs_airwindows_dsp PUBLIC cxx_std_20)

option(LVS_BUILD_EQ "Build LVS Equalizer" ON)
option(LVS_BUILD_COMPRESSOR "Build LVS Compressor" ON)
option(LVS_BUILD_DELAY "Build LVS Delay" ON)
option(LVS_BUILD_REVERB "Build LVS Reverb" ON)
option(LVS_BUILD_LIMITER "Build LVS Limiter" OFF)

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
