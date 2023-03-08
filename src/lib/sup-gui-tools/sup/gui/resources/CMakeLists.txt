set(CMAKE_AUTORCC ON)

target_sources(${library_name} PRIVATE
    sup_gui_tools_icons.qrc
)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
