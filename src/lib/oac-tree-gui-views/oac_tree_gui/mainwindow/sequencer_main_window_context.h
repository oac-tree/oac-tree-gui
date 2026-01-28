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

#ifndef OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_CONTEXT_H_
#define OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_CONTEXT_H_

#include <memory>

namespace sup::gui
{
class IAppCommandService;
}

namespace oac_tree_gui
{

class IDomainPluginService;
class SequencerSettingsModel;

/**
 * @brief The SequencerMainWindowContext contains all common resources for SequencerMainWindow.
 */
class SequencerMainWindowContext
{
public:
  SequencerMainWindowContext();
  ~SequencerMainWindowContext();

  SequencerMainWindowContext(const SequencerMainWindowContext&) = delete;
  SequencerMainWindowContext& operator=(const SequencerMainWindowContext&) = delete;
  SequencerMainWindowContext(SequencerMainWindowContext&&) = delete;
  SequencerMainWindowContext& operator=(SequencerMainWindowContext&&) = delete;

  /**
   * @brief Loads plugins using the plugin service and settings model.
   */
  void LoadPlugins();

  SequencerSettingsModel& GetSettingsModel();

  sup::gui::IAppCommandService& GetCommandService();

  IDomainPluginService& GetDomainPluginService();

private:
  std::unique_ptr<SequencerSettingsModel> m_settings;

  //!< knows about global commands and their shortcuts
  std::unique_ptr<sup::gui::IAppCommandService> m_command_service;

  //!< knows how to load plugins so all instructions and variables get registered
  std::unique_ptr<IDomainPluginService> m_domain_plugin_service;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_CONTEXT_H_
