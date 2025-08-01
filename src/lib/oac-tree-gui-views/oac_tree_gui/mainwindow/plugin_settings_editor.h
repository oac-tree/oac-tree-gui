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

class QTextEdit;

namespace oac_tree_gui
{

/**
 * @brief The SessionItemWidget class is a base for all widgets which can show one item at a time.
 */
class PluginSettingsEditor : public sup::gui::SessionItemWidget
{
  Q_OBJECT

public:
  explicit PluginSettingsEditor(QWidget* parent_widget = nullptr);

  void SetItem(mvvm::SessionItem* item) override;

private:
  QTextEdit* m_text_edit{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_PLUGIN_SETTINGS_EDITOR_H_
