# Collection of files related to the transformations from the GUI model to domain and back.

target_sources(${library_name} PRIVATE
  domain_procedure_builder.cpp
  domain_procedure_builder.h
  domain_workspace_builder.cpp
  domain_workspace_builder.h
  gui_object_builder.cpp
  gui_object_builder.h
  transform_from_domain.cpp
  transform_from_domain.h
  transform_helpers.cpp
  transform_helpers.h
)
