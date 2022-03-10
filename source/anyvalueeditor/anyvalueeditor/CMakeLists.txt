set(library_name anyvalueeditorcore)

set(CMAKE_AUTOMOC ON)

add_library(${library_name} SHARED "")

target_link_libraries(${library_name} PUBLIC pthread MVVM::View COACompact::sup-dto)

target_sources(${library_name} PRIVATE
  anyvalue_type.h
  anyvalueeditoractions.cpp
  anyvalueeditoractions.h
  anyvalueeditortoolbar.cpp
  anyvalueeditortoolbar.h
  anyvalueitem.cpp
  anyvalueitem.h
  anyvalueitembuilder.cpp
  anyvalueitembuilder.h
  editorwidget.cpp
  editorwidget.h
  mainwindow.cpp
  mainwindow.h
  transformfromanyvalue.cpp
  transformfromanyvalue.h
)

target_include_directories(${library_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..$<SEMICOLON>${SEQUENCERGUI_AUTOGEN_DIR}>)
