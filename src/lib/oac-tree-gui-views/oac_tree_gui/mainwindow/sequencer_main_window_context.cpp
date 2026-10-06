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

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/default_command_service.h>
#include <sup/gui/settings/app_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>

namespace oac_tree_gui
{

SequencerMainWindowContext::SequencerMainWindowContext()
    : m_command_service(sup::gui::CreateDefaultCommandService())
    , m_domain_plugin_service(std::make_unique<DomainLibraryLoader>())
    , m_app_settings_storage(sup::gui::CreateAppSettingsStorage())
    , m_settings_manager(std::make_unique<sup::gui::SettingsManager>())
{
  m_settings_manager->SetStorage(sup::gui::SettingsScope::kApplication,
                                 m_app_settings_storage.get());
}

void SequencerMainWindowContext::LoadPlugins()
{
  const auto plugin_settings = m_settings_manager->Get<PluginSettingsInfo>(
      kPluginSettingsGroup, sup::gui::SettingsScope::kApplication,
      CreateDefaultPluginSettingsInfo());

  const auto plugin_file_names = GetPluginFileNames(plugin_settings);
  m_domain_plugin_service->LoadPluginFiles(plugin_file_names);
}

SequencerMainWindowContext::~SequencerMainWindowContext() = default;

sup::gui::IAppCommandService& SequencerMainWindowContext::GetCommandService()
{
  return *m_command_service;
}

sup::gui::AppViewContext SequencerMainWindowContext::GetViewContext()
{
  return {*m_command_service, *m_settings_manager};
}

IDomainPluginService& SequencerMainWindowContext::GetDomainPluginService()
{
  return *m_domain_plugin_service;
}

sup::gui::ISettingsManager& SequencerMainWindowContext::GetSettingsManager()
{
  return *m_settings_manager;
}

void SequencerMainWindowContext::SetProjectSettingsStorage(sup::gui::ISettingsStorage* storage)
{
  m_settings_manager->SetStorage(sup::gui::SettingsScope::kProject, storage);
}

}  // namespace oac_tree_gui
