add_subdirectory(sequencer-gui)
add_subdirectory(libsup-gui-core)

if (NOT SEQUENCERGUI_CODAC)
  add_subdirectory(anyvalue-editor)
  add_subdirectory(sup-pvmonitor)
endif()

