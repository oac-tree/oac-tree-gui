/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <oac_tree_gui/domain/domain_library_loader.h>
#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <sup/gui/app/default_command_service.h>
#include <sup/gui/mainwindow/settings_helper.h>

namespace oac_tree_gui
{

SequencerMainWindowContext::SequencerMainWindowContext()
    : m_settings(std::make_unique<SequencerSettingsModel>())
    , m_command_service(sup::gui::CreateDefaultCommandService())
    , m_domain_plugin_service(std::make_unique<DomainLibraryLoader>())
{
  ::sup::gui::ReadApplicationSettings(*m_settings);
}

void SequencerMainWindowContext::LoadPlugins()
{
  auto plugin_settings = m_settings->Get<PluginSettingsItem>();

  const auto plugin_file_names = GetPluginFileNames(*plugin_settings);
  m_domain_plugin_service->LoadPluginFiles(plugin_file_names);
}

SequencerSettingsModel& SequencerMainWindowContext::GetSettingsModel()
{
  return *m_settings;
}

SequencerMainWindowContext::~SequencerMainWindowContext() = default;

sup::gui::IAppCommandService& SequencerMainWindowContext::GetCommandService()
{
  return *m_command_service;
}

IDomainPluginService& SequencerMainWindowContext::GetDomainPluginService()
{
  return *m_domain_plugin_service;
}

}  // namespace oac_tree_gui
