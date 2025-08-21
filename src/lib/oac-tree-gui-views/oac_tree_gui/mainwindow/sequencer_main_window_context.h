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
class DomainLibraryLoader;
class DomainObjectTypeRegistry;

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

  sup::gui::IAppCommandService& GetCommandService();

  IDomainPluginService& GetDomainPluginService();

private:
  std::unique_ptr<DomainObjectTypeRegistry> CreateObjectTypeRegistry() const;
  std::unique_ptr<IDomainPluginService> CreateDomainPluginService() const;

  //!< knows about global commands and their shortcuts
  std::unique_ptr<sup::gui::IAppCommandService> m_command_service;

  //!< knows how to load libraries and keeps track of what was loaded
  std::unique_ptr<DomainLibraryLoader> m_domain_library_loader;

  //!< knows about all objects registered in plugins, and their types
  std::unique_ptr<DomainObjectTypeRegistry> m_object_type_registry;

  //!< knows how to load plugins, and what objects are registered in them
  //! (use loader and registry from above)
  std::unique_ptr<IDomainPluginService> m_domain_plugin_service;
};

/**
 * @brief Returns the pointer to the global SequencerMainWindowContext.
 *
 * The application context will be available only when main window is created and properly wired
 * from the composition root. In all other cases, this function will return nullptr.
 */
SequencerMainWindowContext* FindSequencerMainWindowContext();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_SEQUENCER_MAIN_WINDOW_CONTEXT_H_
