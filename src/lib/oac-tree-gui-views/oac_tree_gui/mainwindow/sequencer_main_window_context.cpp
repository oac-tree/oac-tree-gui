/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencer_main_window_context.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_library_loader.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>
#include <oac_tree_gui/domain/domain_plugin_service.h>
#include <oac_tree_gui/mainwindow/sequencer_main_window.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <sup/gui/app/default_command_service.h>
#include <sup/gui/mainwindow/settings_helper.h>

#include <sup/oac-tree/instruction_registry.h>
#include <sup/oac-tree/variable_registry.h>

#include <QApplication>

namespace oac_tree_gui
{

SequencerMainWindowContext::SequencerMainWindowContext()
    : m_settings(std::make_unique<SequencerSettingsModel>())
    , m_command_service(sup::gui::CreateDefaultCommandService())
    , m_domain_library_loader(std::make_unique<DomainLibraryLoader>())
    , m_object_type_registry(CreateObjectTypeRegistry())
    , m_domain_plugin_service(CreateDomainPluginService())
{
  ::sup::gui::ReadApplicationSettings(*m_settings);
}

SequencerSettingsModel &SequencerMainWindowContext::GetSettingsModel()
{
  return *m_settings;
}

SequencerMainWindowContext::~SequencerMainWindowContext() = default;

sup::gui::IAppCommandService &SequencerMainWindowContext::GetCommandService()
{
  return *m_command_service;
}

DomainObjectTypeRegistry &SequencerMainWindowContext::GetObjectTypeRegistry()
{
  return *m_object_type_registry;
}

IDomainPluginService &SequencerMainWindowContext::GetDomainPluginService()
{
  return *m_domain_plugin_service;
}

std::unique_ptr<DomainObjectTypeRegistry> SequencerMainWindowContext::CreateObjectTypeRegistry()
    const
{
  // Calls the global registries to get the current list of instructions and variables. This list
  // will grow after each plugin load.
  auto get_object_names = []()
  {
    auto instruction_names =
        ::sup::oac_tree::GlobalInstructionRegistry().RegisteredInstructionNames();
    auto variable_names = ::sup::oac_tree::GlobalVariableRegistry().RegisteredVariableNames();
    std::vector<std::string> names;
    names.reserve(instruction_names.size() + variable_names.size());
    names.insert(names.end(), instruction_names.begin(), instruction_names.end());
    names.insert(names.end(), variable_names.begin(), variable_names.end());
    return names;
  };
  return std::make_unique<DomainObjectTypeRegistry>(get_object_names);
}

std::unique_ptr<IDomainPluginService> SequencerMainWindowContext::CreateDomainPluginService() const
{
  return std::make_unique<DomainPluginService<DomainLibraryLoader, DomainObjectTypeRegistry>>(
      *m_domain_library_loader, *m_object_type_registry);
}

SequencerMainWindowContext *FindSequencerMainWindowContext()
{
  for (auto widget : qApp->topLevelWidgets())
  {
    if (auto result = qobject_cast<SequencerMainWindow *>(widget); result)
    {
      return &result->GetContext();
    }
  }
  return nullptr;
}

}  // namespace oac_tree_gui
