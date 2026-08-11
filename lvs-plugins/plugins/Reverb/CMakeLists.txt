set(LVS_REVERB_SOURCES
    ReverbCids.h
    ReverbProcessor.h
    ReverbProcessor.cpp
    ReverbController.h
    ReverbController.cpp
    ReverbEntry.cpp
    resource/Reverb.uidesc
)

smtg_add_vst3plugin(LVSReverb ${LVS_REVERB_SOURCES})

target_include_directories(LVSReverb PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dsp
)

target_link_libraries(LVSReverb PRIVATE
    sdk
    vstgui_support
    lvs_airwindows_dsp
)

smtg_target_add_plugin_resources(LVSReverb
    RESOURCES
        resource/Reverb.uidesc
)
