# Various reusable components

target_sources(${library_name} PRIVATE
  exceptions.cpp
  exceptions.h
  message_handler_decorator.cpp
  message_handler_decorator.h
  message_handler_factory.cpp
  message_handler_factory.h
  message_handler_interface.h
  standard_message_handlers.cpp
  standard_message_handlers.h
  widget_overlay_message_handler.cpp
  widget_overlay_message_handler.h
)

