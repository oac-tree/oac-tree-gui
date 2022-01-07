set(library_name sequencerguicore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

if (SEQUENCERGUI_CODAC)
  target_link_libraries(${library_name} PUBLIC pthread MVVM::View sequencer ccs-core)
  target_include_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/include $ENV{CODAC_ROOT}/include/sup)
  target_link_directories(${library_name} PUBLIC $ENV{CODAC_ROOT}/lib)
  add_definitions(-DSEQUENCERGUI_CODAC)
else()
  target_link_libraries(${library_name} PUBLIC pthread MVVM::View COACompact::sequencer)
endif()

add_subdirectory(composer)
add_subdirectory(mainwindow)
add_subdirectory(model)
add_subdirectory(monitor)
add_subdirectory(nodeeditor)
add_subdirectory(resources)

target_include_directories(${library_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

