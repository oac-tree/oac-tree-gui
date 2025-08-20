# Widgets for real time operation views

target_sources(${library_name} PRIVATE
  breakpoint_controller.cpp
  breakpoint_controller.h
  breakpoint_helper.cpp
  breakpoint_helper.h
  breakpoint_types.h
  operation_action_context.h
  operation_action_helper.cpp
  operation_action_helper.h
  tooltip_helper.cpp
  tooltip_helper.h
)

add_subdirectory(objects)
