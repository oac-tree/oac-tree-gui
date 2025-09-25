# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  node_connection_controller.cpp
  node_connection_controller.h
  node_graphics_scene.cpp
  node_graphics_scene.h
  node_graphics_scene_v2.cpp
  node_graphics_scene_v2.h
  graphics_scene_component_provider.cpp
  graphics_scene_component_provider.h
)
