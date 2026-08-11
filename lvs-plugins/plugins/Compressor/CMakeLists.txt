set(LVS_COMPRESSOR_SOURCES
    CompressorCids.h
    CompressorProcessor.h
    CompressorProcessor.cpp
    CompressorController.h
    CompressorController.cpp
    CompressorEntry.cpp
    resource/Compressor.uidesc
)

smtg_add_vst3plugin(LVSCompressor ${LVS_COMPRESSOR_SOURCES})

target_include_directories(LVSCompressor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../dsp
)

target_link_libraries(LVSCompressor PRIVATE
    sdk
    vstgui_support
    lvs_airwindows_dsp
)

smtg_target_add_plugin_resources(LVSCompressor
    RESOURCES
        resource/Compressor.uidesc
)
