target_sources(${library_name} PRIVATE
  monitor_model.cpp
  monitor_model.h
  workspace_item_listener.cpp
  workspace_item_listener.h
  workspace_monitor_helper.cpp
  workspace_monitor_helper.h
  workspace_synchronizer.cpp
  workspace_synchronizer.h
)

add_subdirectory(objects)
