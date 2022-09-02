set(library_name anyvalueeditorcore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

target_link_libraries(${library_name} PUBLIC pthread sup-mvvm::view COACompact::sup-dto)

target_sources(${library_name} PRIVATE
  add_field_dialog.cpp
  add_field_dialog.h
  anyvalue_build_adapter.cpp
  anyvalue_build_adapter.h
  anyvalue_editor_actions.cpp
  anyvalue_editor_actions.h
  anyvalue_editor_toolbar.cpp
  anyvalue_editor_toolbar.h
  anyvalue_item.cpp
  anyvalue_item.h
  anyvalue_item_builder.cpp
  anyvalue_item_builder.h
  anyvalue_utils.cpp
  anyvalue_utils.h
  conversion_utils.cpp
  conversion_utils.h
  domain_anyvalue_builder.cpp
  domain_anyvalue_builder.h
  dto_types_fwd.h
  editor_widget.cpp
  editor_widget.h
  main_window.cpp
  main_window.h
  scalar_conversion_utils.cpp
  scalar_conversion_utils.h
)

add_subdirectory(highlighter)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
