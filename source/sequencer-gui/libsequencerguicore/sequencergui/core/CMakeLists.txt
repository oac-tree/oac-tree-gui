# Various reusable components

target_sources(${library_name} PRIVATE
  exceptions.cpp
  exceptions.h
  messagehandlerinterface.h
  messagehandlerdecorator.cpp
  messagehandlerdecorator.h
  messagehandlerfactory.cpp
  messagehandlerfactory.h
  stdmessagehandler.cpp
  stdmessagehandler.h
  widgetoverlaymessagehandler.cpp
  widgetoverlaymessagehandler.h
)

