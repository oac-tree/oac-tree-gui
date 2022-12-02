set(library_name sup-gui-core)

add_library(${library_name} SHARED "")

if (SEQUENCERGUI_CODAC)
  target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sequencer sup-dto)
  target_include_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/include $ENV{CODAC_ROOT}/include/sup)
  target_link_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/lib)
  add_definitions(-DSEQUENCERGUI_CODAC)
else()
  target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sequencer::sequencer)
endif()

add_subdirectory(sup/gui/dto)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
