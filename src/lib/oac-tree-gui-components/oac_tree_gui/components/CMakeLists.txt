# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  anyvalue_dialog_result.h
  component_helper.cpp
  component_helper.h
  component_types.h
  custom_meta_types.cpp
  custom_meta_types.h
  load_resources.cpp
  load_resources.h
  xml_panel_controller.cpp
  xml_panel_controller.h
)

