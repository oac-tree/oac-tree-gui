# Various reusable components

target_sources(${library_name} PRIVATE
  factory.h
  exceptions.cpp
  exceptions.h
  message_handler_factory.cpp
  message_handler_factory.h
  version.h
  widget_overlay_message_handler.cpp
  widget_overlay_message_handler.h
)

