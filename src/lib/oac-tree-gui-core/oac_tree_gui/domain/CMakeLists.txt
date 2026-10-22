# Code related to domain/GUI transformations

target_sources(${library_name} PRIVATE
  attribute_descriptor.h
  domain_automation_helper.cpp
  domain_automation_helper.h
  domain_constants.h
  domain_helper.cpp
  domain_helper.h
  domain_library_loader.cpp
  domain_library_loader.h
  domain_object_group_helper.cpp
  domain_object_group_helper.h
  i_domain_plugin_service.h
  instruction_descriptor.h
  sequencer_types_fwd.h
)

