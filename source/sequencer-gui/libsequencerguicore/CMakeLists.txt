set(library_name sequencerguicore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

if (SEQUENCERGUI_CODAC)
  target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view sequencer ccs-core)
  target_include_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/include $ENV{CODAC_ROOT}/include/sup)
  target_link_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/lib)
  add_definitions(-DSEQUENCERGUI_CODAC)
else()
  target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view COACompact::sequencer COACompact::sequencer-ca COACompact::sequencer-pvxs)
endif()

add_subdirectory(sequencergui)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
