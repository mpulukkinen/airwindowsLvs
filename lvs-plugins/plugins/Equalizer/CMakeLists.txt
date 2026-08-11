set(LVS_EQ_SOURCES
    EqualizerCids.h
    EqualizerProcessor.h
    EqualizerProcessor.cpp
    EqualizerController.h
    EqualizerController.cpp
    EqualizerEntry.cpp
    resource/Equalizer.uidesc
)

smtg_add_vst3plugin(LVSEqualizer ${LVS_EQ_SOURCES})

target_include_directories(LVSEqualizer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dsp
)

target_link_libraries(LVSEqualizer PRIVATE
    sdk
    vstgui_support
    lvs_airwindows_dsp
)

smtg_target_add_plugin_resources(LVSEqualizer
    RESOURCES
        resource/Equalizer.uidesc
)

# Keep the physical bundle name identical to the CMake target. Steinberg's
# moduleinfotool and validator derive the module binary path from that name.
# The VST-visible product name remains "LVS Equalizer" in EqualizerEntry.cpp.
