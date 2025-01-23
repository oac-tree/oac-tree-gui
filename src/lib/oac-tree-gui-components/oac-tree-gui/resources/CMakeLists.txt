set(CMAKE_AUTORCC ON)

target_sources(${library_name} PRIVATE
    oac_tree_icons.qrc
)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

install(FILES icons/sequencer-gui-96.png DESTINATION share/sup/icons)
install(FILES icons/sequencer-operation-96.png DESTINATION share/sup/icons)
install(FILES icons/sup-pvmonitor-96.png DESTINATION share/sup/icons)
