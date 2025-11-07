target_sources(${library_name} PRIVATE
  align_node.cpp
  align_node.h
  align_utils.cpp
  align_utils.h
  connectable_instruction_adapter.cpp
  connectable_instruction_adapter.h
  connectable_shape_factory.cpp
  connectable_shape_factory.h
  graphics_scene_action_handler.cpp
  graphics_scene_action_handler.h
  graphics_scene_types.h
  i_connectable_view_factory.h
  i_graphics_scene_action_handler.h
  scene_constants.h
  scene_utils.cpp
  scene_utils.h
  sequencer_align_utils.cpp
  sequencer_align_utils.h
)

add_subdirectory(objects)
