set(library_name sequencer-gui-core)

add_library(${library_name} SHARED "")

if (SEQUENCERGUI_CODAC)
  target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sequencer sup-dto sup-gui-core)
  target_include_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/include $ENV{CODAC_ROOT}/include/sup)
  target_link_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/lib)
  add_definitions(-DSEQUENCERGUI_CODAC)
else()
  target_link_libraries(${library_name} PUBLIC pthread sequencer::sequencer sup-gui-core)
endif()

add_subdirectory(sequencergui)

message(INFO "AAA ${SEQUENCERGUI_AUTOGEN_DIR}")
target_include_directories(${library_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<BUILD_INTERFACE:${SEQUENCERGUI_AUTOGEN_DIR}>)
