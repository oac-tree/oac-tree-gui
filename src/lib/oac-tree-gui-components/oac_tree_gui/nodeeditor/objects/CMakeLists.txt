# This folder contains QObject derived classes (they are excluded from the Sonar analysis)

target_sources(${library_name} PRIVATE
  node_graphics_scene.cpp
  node_graphics_scene.h
  graphics_scene_component_provider.cpp
  graphics_scene_component_provider.h
)
