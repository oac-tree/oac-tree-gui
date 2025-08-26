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

#include "plugin_settings_text_controller.h"

#include <oac_tree_gui/model/plugin_settings_item.h>

#include <oac_tree_gui/core/exceptions.h>

#include <QCheckBox>
#include <QTextEdit>

namespace oac_tree_gui
{

PluginSettingsTextController::PluginSettingsTextController(const TextControllerContext &context)
    : m_context(context)
{
  if (!context.check_box)
  {
    throw RuntimeException("QCheckBox is not initialized");
  }

  if (!context.text_edit)
  {
    throw RuntimeException("QTextEdit is not initialized");
  }
}

void PluginSettingsTextController::Subscribe()
{
  UpdateWidgetStateToItem();
}

void PluginSettingsTextController::UpdateWidgetStateToItem()
{
  m_context.check_box->setChecked(GetItem()->Property<bool>(m_context.check_box_property));
  m_context.text_edit->setEnabled(m_context.check_box->isChecked());
  // m_context.text_edit->setPlainText(
  //     QString::fromStdString(GetItem()->Property<std::string>(m_context.text_edit_property)
}

}  // namespace oac_tree_gui
