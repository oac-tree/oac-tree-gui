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

#include "plugin_settings_editor.h"

#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/sequencer_settings_model.h>

#include <mvvm/utils/string_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

QString GetFormattedContent(const std::string &str)
{
  auto formatted = mvvm::utils::ReplaceSubString(str, ",", "\n");
  return QString::fromStdString(formatted);
}

}  // namespace

PluginSettingsEditor::PluginSettingsEditor(QWidget *parent_widget)
    : sup::gui::SessionItemWidget(parent_widget), m_text_edit(new QTextEdit)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(mvvm::utils::UnitSize(0.5), 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_text_edit);
}

void PluginSettingsEditor::SetItem(mvvm::SessionItem *item)
{
  const QString content =
      GetFormattedContent(item->GetItem(constants::kPluginList)->Data<std::string>());
  m_text_edit->setText(content);
}

}  // namespace oac_tree_gui
