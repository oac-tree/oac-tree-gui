# Contains source for SequencerExplorerView.
# This is one of 3 main views of MainWindow.

target_sources(${library_name} PRIVATE
  explorerpanel.cpp
  explorerpanel.h
  proceduretreeswidget.cpp
  proceduretreeswidget.h
  sequencerexplorerview.cpp
  sequencerexplorerview.h
  xmleditor.cpp
  xmleditor.h
  xmlsyntaxhighlighter.cpp
  xmlsyntaxhighlighter.h
)
