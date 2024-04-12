# Collection of general purpose widgets and objects that don't contain any Sequencer UI related
# specifics.

target_sources(${library_name} PRIVATE
  custom_tree_view_style.cpp
  custom_tree_view_style.h
  dialog_message_handler.cpp
  dialog_message_handler.h
  file_tree_view.cpp
  file_tree_view.h
  overlay_message.cpp
  overlay_message.h
  overlay_message_frame.cpp
  overlay_message_frame.h
  overlay_widget_position_strategy.cpp
  overlay_widget_position_strategy.h
  steady_menu.cpp
  steady_menu.h
  tree_helper.cpp
  tree_helper.h
  warning_sign_widget.cpp
  warning_sign_widget.h
  widget_overlay_message_handler.cpp
  widget_overlay_message_handler.h
  widget_overlay_message_publisher.cpp
  widget_overlay_message_publisher.h
)
