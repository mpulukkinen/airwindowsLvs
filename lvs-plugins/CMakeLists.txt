cmake_minimum_required(VERSION 3.22)

project(LVSAudioSuite VERSION 0.2.0 LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "LVS Audio Suite currently supports Windows only")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SMTG_CREATE_PLUGIN_LINK OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SMTG_ENABLE_VSTGUI_SUPPORT ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    vst3sdk
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
    GIT_TAG 58f8da7936800732561402d7936584ca4505de07
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

set(AIRWINDOWS_WINVST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../plugins/WinVST")
set(LVS_AIRWINDOWS_LEGACY_ALGORITHMS
    TapeDelay2
    Reverb
    Recurve
    ADClip9
    DeBess
    SoftGate
    StereoChorus
    Wider
    Density3
    ToTape9
    StereoDoubler
    BiquadDouble
    Tremolo
    AutoPan
    Air3
    Drive
    Podcast
    StereoEnsemble
    PitchDelay
    RingModulator
    DeRez4
    FireAmp
)

set(LVS_AIRWINDOWS_LEGACY_SOURCES)
set(LVS_AIRWINDOWS_LEGACY_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/legacy
)

foreach(ALGORITHM IN LISTS LVS_AIRWINDOWS_LEGACY_ALGORITHMS)
    set(ALGORITHM_DIR "${AIRWINDOWS_WINVST_ROOT}/${ALGORITHM}")
    set(ALGORITHM_MAIN "${ALGORITHM_DIR}/${ALGORITHM}.cpp")
    set(ALGORITHM_PROC "${ALGORITHM_DIR}/${ALGORITHM}Proc.cpp")

    set_source_files_properties("${ALGORITHM_MAIN}" PROPERTIES
        COMPILE_DEFINITIONS "createEffectInstance=create${ALGORITHM}EffectInstance"
    )

    list(APPEND LVS_AIRWINDOWS_LEGACY_SOURCES
        "${ALGORITHM_MAIN}"
        "${ALGORITHM_PROC}"
    )
    list(APPEND LVS_AIRWINDOWS_LEGACY_INCLUDE_DIRS "${ALGORITHM_DIR}")
endforeach()

add_library(lvs_airwindows_legacy STATIC
    ${LVS_AIRWINDOWS_LEGACY_SOURCES}
)
target_include_directories(lvs_airwindows_legacy PUBLIC
    ${LVS_AIRWINDOWS_LEGACY_INCLUDE_DIRS}
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

add_library(lvs_legacy_vst3_common STATIC
    common/LegacyAirwindowsVst3.cpp
)
target_include_directories(lvs_legacy_vst3_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/legacy
)
target_link_libraries(lvs_legacy_vst3_common PUBLIC
    sdk
    vstgui_support
)
target_compile_features(lvs_legacy_vst3_common PUBLIC cxx_std_20)

option(LVS_BUILD_EQ "Build LVS Equalizer" ON)
option(LVS_BUILD_COMPRESSOR "Build LVS Compressor" ON)
option(LVS_BUILD_DELAY "Build LVS Delay" ON)
option(LVS_BUILD_REVERB "Build LVS Reverb" ON)
option(LVS_BUILD_EXTENDED "Build the expanded LVS Audio Suite effects" ON)

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

function(lvs_add_extended_plugin TARGET KEY ALGORITHM)
    set(PLUGIN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/Extended/${KEY}.cpp")
    set(PLUGIN_UIDESC "${CMAKE_CURRENT_SOURCE_DIR}/plugins/Extended/resource/${KEY}.uidesc")

    smtg_add_vst3plugin(${TARGET}
        ${PLUGIN_SOURCE}
        ${PLUGIN_UIDESC}
    )

    target_include_directories(${TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/legacy
        ${AIRWINDOWS_WINVST_ROOT}/${ALGORITHM}
    )

    target_link_libraries(${TARGET} PRIVATE
        sdk
        vstgui_support
        lvs_legacy_vst3_common
        lvs_airwindows_legacy
    )

    smtg_target_add_plugin_resources(${TARGET}
        RESOURCES
            ${PLUGIN_UIDESC}
    )
endfunction()

if(LVS_BUILD_EXTENDED)
    lvs_add_extended_plugin(LVSLimiter Limiter Recurve)
    lvs_add_extended_plugin(LVSClipper Clipper ADClip9)
    lvs_add_extended_plugin(LVSDeEsser DeEsser DeBess)
    lvs_add_extended_plugin(LVSGate Gate SoftGate)
    lvs_add_extended_plugin(LVSChorus Chorus StereoChorus)
    lvs_add_extended_plugin(LVSStereoWidth StereoWidth Wider)
    lvs_add_extended_plugin(LVSSaturation Saturation Density3)
    lvs_add_extended_plugin(LVSTape Tape ToTape9)
    lvs_add_extended_plugin(LVSDoubler Doubler StereoDoubler)
    lvs_add_extended_plugin(LVSFilter Filter BiquadDouble)
    lvs_add_extended_plugin(LVSTremolo Tremolo Tremolo)
    lvs_add_extended_plugin(LVSAutoPan AutoPan AutoPan)
    lvs_add_extended_plugin(LVSExciter Exciter Air3)
    lvs_add_extended_plugin(LVSDistortion Distortion Drive)
    lvs_add_extended_plugin(LVSVocalLeveler VocalLeveler Podcast)
    lvs_add_extended_plugin(LVSEnsemble Ensemble StereoEnsemble)
    lvs_add_extended_plugin(LVSPitchDelay PitchDelay PitchDelay)
    lvs_add_extended_plugin(LVSRingModulator RingMod RingModulator)
    lvs_add_extended_plugin(LVSLoFi LoFi DeRez4)
    lvs_add_extended_plugin(LVSGuitarAmp Amp FireAmp)
endif()
