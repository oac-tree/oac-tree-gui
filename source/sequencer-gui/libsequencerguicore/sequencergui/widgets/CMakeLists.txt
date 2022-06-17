# Collection of general purpose widgets that doesn't contain any Sequencer UI related specifics.

target_sources(${library_name} PRIVATE
  collapsible_list_view.cpp
  collapsible_list_view.h
  collapsible_toolbar.cpp
  collapsible_toolbar.h
  collapsible_widget.cpp
  collapsible_widget.h
  custom_header_view.cpp
  custom_header_view.h
  dialog_message_handler.cpp
  dialog_message_handler.h
  dots_toolbar.cpp
  dots_toolbar.h
  file_tree_view.cpp
  file_tree_view.h
  item_list_widget.cpp
  item_list_widget.h
  overlay_message.cpp
  overlay_message.h
  overlay_message_frame.cpp
  overlay_message_frame.h
  overlay_widget_position_strategy.cpp
  overlay_widget_position_strategy.h
  panel_toolbar.cpp
  panel_toolbar.h
  warning_sign_widget.cpp
  warning_sign_widget.h
  widget_overlay_message_publisher.cpp
  widget_overlay_message_publisher.h
  widget_utils.cpp
  widget_utils.h
  )
