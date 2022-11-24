add_subdirectory(sequencer-gui)

if (NOT SEQUENCERGUI_CODAC)
  add_subdirectory(anyvalue-editor)
  add_subdirectory(sup-pvmonitor)
endif()

