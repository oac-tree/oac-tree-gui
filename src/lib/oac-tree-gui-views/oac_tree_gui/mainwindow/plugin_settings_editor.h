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

#ifndef OAC_TREE_GUI_MAINWINDOW_PLUGIN_SETTINGS_EDITOR_H_
#define OAC_TREE_GUI_MAINWINDOW_PLUGIN_SETTINGS_EDITOR_H_

#include <sup/gui/mainwindow/session_item_widget.h>

#include <memory>

class QTextEdit;
class QCheckBox;
class QLayout;

namespace oac_tree_gui
{

class PluginSettingsItem;
class TextEditController;

/**
 * @brief The PluginSettingsEditor class is a custom editor for PluginSettingsItem
 */
class PluginSettingsEditor : public sup::gui::SessionItemWidget
{
  Q_OBJECT

public:
  explicit PluginSettingsEditor(QWidget* parent_widget = nullptr);
  virtual ~PluginSettingsEditor() override;

  void SetItem(mvvm::SessionItem* item) override;

private:  
  void SetPluginSettingsItem(PluginSettingsItem* item);

  /**
   * @brief Returns layout with general information.
   */
  std::unique_ptr<QLayout> CreateDescriptionLayout();

  /**
   * @brief Returns layout to edit plugin directory list.
   */
  std::unique_ptr<QLayout> CreateDirListLayout();

  /**
   * @brief Returns layout to edit plugin filenames list.
   */
  std::unique_ptr<QLayout> CreatePluginListLayout();

  QCheckBox* m_dir_list_checkbox{nullptr};
  QTextEdit* m_dir_list_edit{nullptr};

  QCheckBox* m_plugin_list_checkbox{nullptr};
  QTextEdit* m_plugin_list_edit{nullptr};

  std::unique_ptr<TextEditController> m_dir_list_controller{nullptr};
  std::unique_ptr<TextEditController> m_plugin_list_controller{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_PLUGIN_SETTINGS_EDITOR_H_
