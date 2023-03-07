target_sources(${library_name} PRIVATE
  anyvalue_editor.cpp
  anyvalue_editor.h
  anyvalue_editor_actions.cpp
  anyvalue_editor_actions.h
  anyvalue_editor_context.h
  anyvalue_editor_textpanel.cpp
  anyvalue_editor_textpanel.h
  anyvalue_editor_toolbar.cpp
  anyvalue_editor_toolbar.h
)

add_subdirectory(highlighter)
