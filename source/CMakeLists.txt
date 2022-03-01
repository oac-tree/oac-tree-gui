add_subdirectory(sequencer-gui)

if (NOT SEQUENCERGUI_CODAC)
  add_subdirectory(anyvalueeditor)
endif()

