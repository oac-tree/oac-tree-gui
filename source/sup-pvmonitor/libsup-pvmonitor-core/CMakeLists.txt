set(library_name sup-pvmonitor-core)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

add_subdirectory(suppvmonitor)

target_link_libraries(${library_name} PUBLIC pthread sequencer-gui-core)

message(INFO "AAA ${SEQUENCERGUI_AUTOGEN_DIR}")
target_include_directories(${library_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SEQUENCERGUI_AUTOGEN_DIR}>)
