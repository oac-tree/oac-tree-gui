set(CMAKE_AUTORCC ON)

target_sources(${library_name} PRIVATE
    sequencericons.qrc
)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
