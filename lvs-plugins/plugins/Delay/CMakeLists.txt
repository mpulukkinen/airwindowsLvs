set(LVS_DELAY_SOURCES
    DelayCids.h
    DelayProcessor.h
    DelayProcessor.cpp
    DelayController.h
    DelayController.cpp
    DelayEntry.cpp
    resource/Delay.uidesc
)

smtg_add_vst3plugin(LVSDelay ${LVS_DELAY_SOURCES})

target_include_directories(LVSDelay PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dsp
)

target_link_libraries(LVSDelay PRIVATE
    sdk
    vstgui_support
    lvs_airwindows_dsp
)

smtg_target_add_plugin_resources(LVSDelay
    RESOURCES
        resource/Delay.uidesc
)
