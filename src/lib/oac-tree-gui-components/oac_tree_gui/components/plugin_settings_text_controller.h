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

#ifndef OAC_TREE_GUI_COMPONENTS_PLUGIN_SETTINGS_TEXT_CONTROLLER_H_
#define OAC_TREE_GUI_COMPONENTS_PLUGIN_SETTINGS_TEXT_CONTROLLER_H_

#include <mvvm/signals/item_controller.h>

#include <string>

class QTextEdit;
class QCheckBox;

namespace oac_tree_gui
{

/**
 * @brief The TextControllerContext is a collection of data to initialize
 * PluginSettingsTextController.
 */
struct TextControllerContext
{
  //! property of PluginSettingsItem which holds the value for QCheckBox
  std::string check_box_property;

  //! Qt check box to control
  QCheckBox* check_box{nullptr};

  //! property of PluginSettingsItem which holds the data for QTextEditor
  std::string text_edit_property;

  //! Qt QTextEditor to control
  QTextEdit* text_edit{nullptr};
};

class PluginSettingsItem;

/**
 * @brief The PluginSettingsTextController class is a controller to synchronize
 * properties of PluginSettingsItem with QCheckBox and QTextEdit.
 *
 * The tast of the controller is to enable/disable QTextEdit depending on the state of QCheckBox
 * and to synchronize values of both widgets with corresponding properties of PluginSettingsItem.
 */
class PluginSettingsTextController : public mvvm::ItemController<PluginSettingsItem>
{
public:
  explicit PluginSettingsTextController(const TextControllerContext& context);

protected:
  void Subscribe() override;

private:
  void UpdateWidgetStateToItem();
  TextControllerContext m_context;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_PLUGIN_SETTINGS_TEXT_CONTROLLER_H_
