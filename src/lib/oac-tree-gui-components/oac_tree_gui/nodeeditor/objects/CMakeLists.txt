# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  node_connection_controller.cpp
  node_connection_controller.h
  node_graphics_scene.cpp
  node_graphics_scene.h
)
