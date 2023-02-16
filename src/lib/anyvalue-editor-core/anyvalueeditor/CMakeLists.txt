target_sources(${library_name} PRIVATE
  anyvalue_editor_actions.cpp
  anyvalue_editor_actions.h
  anyvalue_editor_toolbar.cpp
  anyvalue_editor_toolbar.h
  editor_widget.cpp
  editor_widget.h
)

add_subdirectory(highlighter)
