# Collection of files related to the transformations from the GUI model to domain and back.

target_sources(${library_name} PRIVATE
  anyvalue_item_transform_helper.cpp
  anyvalue_item_transform_helper.h
  attribute_item_transform_helper.cpp
  attribute_item_transform_helper.h
  domain_procedure_builder.cpp
  domain_procedure_builder.h
  domain_workspace_builder.cpp
  domain_workspace_builder.h
  i_procedure_item_builder.h
  instruction_item_transform_helper.cpp
  instruction_item_transform_helper.h
  procedure_item_job_info_builder.cpp
  procedure_item_job_info_builder.h
  procedure_item_transform_helper.cpp
  procedure_item_transform_helper.h
  transform_from_domain.cpp
  transform_from_domain.h
  variable_item_transform_helper.cpp
  variable_item_transform_helper.h
)
